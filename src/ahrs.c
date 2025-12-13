#include "ahrs.h"
#include <math.h>

// AHRS internal state (radians)
static double roll = 0.0;
static double pitch = 0.0;
static double yaw = 0.0;

// Complementary filter constant
static const double alpha = 0.98;

void ahrs_init(void) {
    roll = 0.0;
    pitch = 0.0;
    yaw = 0.0;
}

void ahrs_update(
    double ax, double ay, double az,
    double gx, double gy, double gz,
    double dt_s,
    double *roll_deg,
    double *pitch_deg,
    double *heading_deg
) {
    // Accelerometer-based angles (radians)
    double roll_acc  = atan2(ay, az);
    double pitch_acc = atan2(-ax, sqrt(ay*ay + az*az));

    // Gyro: deg/s → rad/s
    double gx_rad = gx * M_PI / 180.0;
    double gy_rad = gy * M_PI / 180.0;
    double gz_rad = gz * M_PI / 180.0;

    // Complementary filter
    roll  = alpha * (roll  + gx_rad * dt_s) + (1.0 - alpha) * roll_acc;
    pitch = alpha * (pitch + gy_rad * dt_s) + (1.0 - alpha) * pitch_acc;

    // Heading (yaw) — gyro only
    yaw += gz_rad * dt_s;

    // Output in degrees
    *roll_deg     = roll  * 180.0 / M_PI;
    *pitch_deg   = pitch * 180.0 / M_PI;
    *heading_deg = yaw   * 180.0 / M_PI;
}
