/* telemetry_tx.c
   Build: gcc -O2 -o telemetry_tx telemetry_tx.c -lm
   Run: telemetry_tx.exe --stdout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

double now_ms() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    unsigned long long v = ((unsigned long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    v -= 116444736000000000ULL;
    return (double)(v / 10000.0);
}

unsigned char xor_checksum(const char *s, size_t len) {
    unsigned char chk = 0;
    for (size_t i = 0; i < len; ++i) chk ^= (unsigned char)s[i];
    return chk;
}

int main() {
    double t0 = now_ms();
    const double dt = 50.0; // 20 Hz
    const double two_pi = 2.0 * M_PI;

    while (1) {
        double ts = now_ms() - t0;

        double ax = 0.1 * sin(two_pi * 0.5 * (ts / 1000.0));
        double ay = 0.1 * cos(two_pi * 0.5 * (ts / 1000.0));
        double az = 1.0;

        double gx = 0.5 * sin(two_pi * 0.2 * (ts / 1000.0));
        double gy = 0.4 * cos(two_pi * 0.15 * (ts / 1000.0));
        double gz = 0.2 * sin(two_pi * 0.1 * (ts / 1000.0));

        double alt = 100.0 + 5.0 * sin(two_pi * 0.05 * (ts / 1000.0));
        double temp = 25.0;

        char payload[256];
        int n = snprintf(payload, sizeof(payload),
            "L1,%.0f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.3f,%.3f",
            ts, ax, ay, az, gx, gy, gz, alt, temp);

        unsigned char chk = xor_checksum(payload, n);

        char framebuf[320];
        int m = snprintf(framebuf, sizeof(framebuf), "$%s*%02X\n", payload, chk);

        fwrite(framebuf, 1, m, stdout);
        fflush(stdout);

        Sleep((DWORD)dt);
    }
}
