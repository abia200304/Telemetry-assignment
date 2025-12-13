import sys
import time
import matplotlib.pyplot as plt
from collections import deque

MAX_POINTS = 200

time_buf = deque(maxlen=MAX_POINTS)
roll_buf = deque(maxlen=MAX_POINTS)
pitch_buf = deque(maxlen=MAX_POINTS)
yaw_buf = deque(maxlen=MAX_POINTS)

plt.ion()
fig, ax = plt.subplots()

line_roll, = ax.plot([], [], label="Roll")
line_pitch, = ax.plot([], [], label="Pitch")
line_yaw, = ax.plot([], [], label="Yaw")

ax.set_xlabel("Time (s)")
ax.set_ylabel("Angle (deg)")
ax.set_title("Live AHRS Orientation")
ax.legend()
ax.grid(True)

start_time = None

def xor_checksum(b):
    chk = 0
    for x in b:
        chk ^= x
    return chk

def parse_line(line):
    line = line.strip()
    if not line.startswith("$") or "*" not in line:
        return None

    core, chk = line[1:].split("*", 1)
    try:
        if xor_checksum(core.encode()) != int(chk, 16):
            return None
    except:
        return None

    fields = core.split(",")
    if fields[0] != "L2" or len(fields) != 7:
        return None

    return {
        "t": float(fields[1]) / 1000.0,
        "roll": float(fields[2]),
        "pitch": float(fields[3]),
        "yaw": float(fields[4])
    }

print("Waiting for L2 telemetry...")

while True:
    line = sys.stdin.readline()
    if not line:
        time.sleep(0.01)
        continue

    frame = parse_line(line)
    if not frame:
        continue

    if start_time is None:
        start_time = frame["t"]

    t = frame["t"] - start_time

    time_buf.append(t)
    roll_buf.append(frame["roll"])
    pitch_buf.append(frame["pitch"])
    yaw_buf.append(frame["yaw"])

    line_roll.set_data(time_buf, roll_buf)
    line_pitch.set_data(time_buf, pitch_buf)
    line_yaw.set_data(time_buf, yaw_buf)

    ax.relim()
    ax.autoscale_view()

    plt.pause(0.01)
