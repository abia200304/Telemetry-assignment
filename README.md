# Telemetry Firmware Assignment (Level 1 & Level 2)

## Overview
This project implements a telemetry pipeline using simulated sensor data.
A C-based transmitter generates UART-style telemetry frames, and Python tools
receive, parse, log, and visualize the data.

---

## Level 1 – Telemetry
- Simulated accelerometer, gyroscope, altitude, and temperature
- UART ASCII frame format
- XOR checksum validation
- Python receiver logs data to CSV

### Frame Format
$L1,timestamp_ms,ax,ay,az,gx,gy,gz,alt,temp*CHK

---

## Level 2 – AHRS
- Complementary filter AHRS
- Computes roll, pitch, and heading
- Modular AHRS implementation (`ahrs.c / ahrs.h`)
- Level-2 telemetry protocol

### Level 2 Frame Format
$L2,timestamp_ms,roll,pitch,heading,alt,temp*CHK

---

## Files
- telemetry_tx.c – Telemetry transmitter
- ahrs.c / ahrs.h – AHRS complementary filter
- uart_rx.py – Telemetry receiver and CSV logger
- plot_live.py – Live visualization (matplotlib)
- output.csv – Sample telemetry output

---

## Build & Run (Windows)

### Compile
```cmd
gcc -O2 telemetry_tx.c ahrs.c -o telemetry_tx -lm

### Run transmitter
```cmd
telemetry_tx.exe --stdout
### Visualization
telemetry_tx.exe --stdout | py plot_live.py


