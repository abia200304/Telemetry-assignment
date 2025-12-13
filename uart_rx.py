#!/usr/bin/env python3
# uart_rx.py
# Usage (Windows):
#   type frames.txt | py uart_rx.py x output.csv
#
# The first argument (port) is ignored. The script always reads from stdin.

import sys
import time
import csv
import argparse


# --------------------------------------------------
# Compute XOR checksum of ASCII bytes
# --------------------------------------------------
def xor_checksum(b: bytes) -> int:
    chk = 0
    for x in b:
        chk ^= x
    return chk


# --------------------------------------------------
# Read one line from stdin (blocking)
# --------------------------------------------------
def read_line_from_stdin():
    line = sys.stdin.buffer.readline()
    if not line:
        return None
    return line.decode('ascii', errors='ignore')


# --------------------------------------------------
# Parse a telemetry frame
# --------------------------------------------------
def process_frame(line):
    line = line.strip()

    if not line.startswith("$") or "*" not in line:
        return None

    try:
        core, chk_str = line[1:].split("*", 1)
    except ValueError:
        return None

    calc = xor_checksum(core.encode('ascii'))

    try:
        recv = int(chk_str, 16)
    except:
        return None

    if calc != recv:
        print(f"Checksum mismatch: calc={calc:02X} recv={recv:02X}")
        return None

    fields = core.split(",")

    if fields[0] != "L1" or len(fields) != 10:
        return None

    try:
        ts = float(fields[1])
        ax, ay, az = map(float, fields[2:5])
        gx, gy, gz = map(float, fields[5:8])
        alt = float(fields[8])
        temp = float(fields[9])
    except:
        return None

    return {
        "timestamp_ms": ts,
        "ax": ax, "ay": ay, "az": az,
        "gx": gx, "gy": gy, "gz": gz,
        "alt": alt, "temp": temp
    }


# --------------------------------------------------
# MAIN
# --------------------------------------------------
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("port", help="ignored")
    parser.add_argument("csvfile", help="CSV file to save data")
    args = parser.parse_args()

    with open(args.csvfile, "a", newline="") as csvfile:
        fieldnames = ["timestamp_ms", "ax", "ay", "az", "gx", "gy", "gz", "alt", "temp"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        # Write header if file is empty
        csvfile.seek(0, 2)
        if csvfile.tell() == 0:
            writer.writeheader()

        print("Receiver started... Waiting for data...")

        while True:
            line = read_line_from_stdin()

            if not line:
                time.sleep(0.01)
                continue

            frame = process_frame(line)
            if frame:
                print(
                    f"[{frame['timestamp_ms']:8.0f} ms] "
                    f"a=({frame['ax']:.3f}, {frame['ay']:.3f}, {frame['az']:.3f}) "
                    f"g=({frame['gx']:.3f}, {frame['gy']:.3f}, {frame['gz']:.3f}) "
                    f"alt={frame['alt']:.2f} temp={frame['temp']:.2f}"
                )

                writer.writerow(frame)
                csvfile.flush()


if __name__ == "__main__":
    main()
