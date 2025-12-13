# Telemetry UART Assignment – Level 1

## Description
This project implements a Level-1 telemetry system using:
- A C-based transmitter that simulates sensor data at ~20 Hz
- A Python receiver that validates checksum, parses frames, and logs data

## Files
- telemetry_tx.c – C transmitter (simulated sensors)
- uart_rx.py – Python receiver
- output.csv – Sample output log

## Frame Format
$L1,timestamp,ax,ay,az,gx,gy,gz,alt,temp*CHK

Checksum (CHK) is XOR of all ASCII characters between `$` and `*`.

## How to Run (Windows)
```cmd
telemetry_tx.exe --stdout > frames.txt
type frames.txt | py uart_rx.py x output.csv
