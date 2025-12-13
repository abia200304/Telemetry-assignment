/* telemetry_tx.c
   Build: gcc -O2 telemetry_tx.c ahrs.c -o telemetry_tx -lm
   Run: telemetry_tx.exe --stdout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "ahrs.h"

double now_ms() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    unsigned long long v =
        ((unsigned long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    v -= 116444736000000000ULL;
    return (double)(v / 10000.0);
}

unsigned char xor_checksum(const char *s, size_t len) {
    unsigned char chk = 0;
    for (size_t i = 0; i < len; ++i)
        chk ^= (unsigned char)s[i];
    return chk;
}

int main() {
    double t0 = now_ms();

    const double dt_ms = 50.0;   // 20 Hz
    const double dt_s  = 0.05;
    const double two_pi = 2.0 * M_PI;

    // Initialize AHRS module
    ahrs_init();

    while (1) {
        double ts = now_ms() - t0;

        // ===== Simulated sensors =====
        double ax = 0.1 * sin(two_pi * 0.5 * (ts / 1000.0));
        double ay = 0.1 * cos(two_pi * 0.5 * (ts / 1000.0));
        double az = 1.0;

        // Gyro (deg/sec)
        double gx = 0.5 * sin(two_pi * 0.2 * (ts / 1000.0));
        double gy = 0.4 * cos(two_pi * 0.15 * (ts / 1000.0));
        double gz = 0.2 * sin(two_pi * 0.1 * (ts / 1000.0));

        double alt  = 100.0 + 5.0 * sin(two_pi * 0.05 * (ts / 1000.0));
        double temp = 25.0;

        // ===== Call AHRS module =====
        double roll_deg, pitch_deg, heading_deg;

        ahrs_update(
            ax, ay, az,
            gx, gy, gz,
            dt_s,
            &roll_deg,
            &pitch_deg,
            &heading_deg
        );

        // ===== Level-2 telemetry frame =====
        char payload[256];
        int n = snprintf(payload, sizeof(payload),
            "L2,%.0f,%.2f,%.2f,%.2f,%.2f,%.2f",
            ts, roll_deg, pitch_deg, heading_deg, alt, temp);

        unsigned char chk = xor_checksum(payload, n);

        char framebuf[320];
        int m = snprintf(framebuf, sizeof(framebuf),
            "$%s*%02X\n", payload, chk);

        fwrite(framebuf, 1, m, stdout);
        fflush(stdout);

        Sleep((DWORD)dt_ms);
    }
}
