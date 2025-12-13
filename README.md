# Level 1 Telemetry Firmware Task

## Overview
This project implements a Level-1 telemetry pipeline using simulated sensor data.
A C program generates telemetry frames at 20 Hz, and a Python script receives,
validates, parses, and logs the data.

## Sensor Data
Sensors are simulated using sine/cosine waveforms to represent predictable motion.
The following sensors are generated:
- Accelerometer (ax, ay, az)
- Gyroscope (gx, gy, gz)
- Altitude
- Temperature

Magnetometer data is simulated internally but not transmitted, as it is not part
of the Level-1 frame format.

## Telemetry Frame Format
Each frame is sent every 50 ms (20 Hz) in ASCII format:

$L1,timestamp_ms,ax,ay,az,gx,gy,gz,alt,temp*CHK

The checksum (CHK) is computed as the XOR of all ASCII characters between `$` and `*`.

## Files
- telemetry_tx.c — C telemetry transmitter
- uart_rx.py — Python receiver and logger
- output.csv — Sample logged telemetry data

## How to Compile and Run (Windows)

### Compile
```cmd
gcc -O2 -o telemetry_tx telemetry_tx.c -lm
