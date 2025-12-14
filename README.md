# Telemetry Firmware Assignment (Level 1 & Level 2)

## Overview
This project implements a telemetry pipeline using simulated sensor data.
A C-based transmitter generates UART-style telemetry frames, and Python tools
receive, parse, log, and visualize the data.

The project includes:
- Level 1: Telemetry generation and reception
- Level 2: AHRS computation and real-time visualization

---

## Level 1 – Telemetry

### Sensor Data (Simulated)
The following sensor values are simulated using sine and cosine functions to
represent predictable motion:
- Accelerometer (ax, ay, az)
- Gyroscope (gx, gy, gz)
- Altitude
- Temperature

### Telemetry Frame Format
$L1,timestamp_ms,ax,ay,az,gx,gy,gz,alt,temp*CHK


The checksum (CHK) is calculated as the XOR of all ASCII characters between
`$` and `*`.

---

## Level 2 – AHRS

### AHRS Algorithm
A complementary filter is used to estimate orientation:
- Roll
- Pitch
- Heading (yaw)

Accelerometer data provides long-term stability, while gyroscope integration
provides short-term accuracy.

Magnetometer data is not used; therefore, heading may drift over time.
This limitation is documented and acceptable for Level 2.

### Level 2 Telemetry Frame Format
$L2,timestamp_ms,roll,pitch,heading,alt,temp*CHK

---

## Files
- telemetry_tx.c – Telemetry transmitter
- ahrs.c / ahrs.h – AHRS complementary filter implementation
- plot_live.py – Real-time visualization (matplotlib)

---

## Build & Run (Windows)

### Compile
gcc -O2 telemetry_tx.c ahrs.c -o telemetry_tx -lm



### Run transmitter
telemetry_tx.exe --stdout | py plot_live.py

---

## Performance
- Telemetry rate: ~20 Hz
- Visualization update rate: ~10–20 Hz
- Roll and pitch are stable
- Heading drifts slowly over time

---

## Limitations
- No magnetometer correction
- Heading drift over time
- Sensor data is simulated (no real hardware)

---

## AI Usage
ChatGPT was used for guidance on AHRS theory, debugging, refactoring,
and documentation structure. All code was manually tested and validated.
