#ifndef AHRS_H
#define AHRS_H

// Initialize AHRS state
void ahrs_init(void);

// Update AHRS using complementary filter
void ahrs_update(
    double ax, double ay, double az,
    double gx, double gy, double gz,
    double dt_s,
    double *roll_deg,
    double *pitch_deg,
    double *heading_deg
);

#endif
