#!/usr/bin/env python3
import serial
import random
import time

PORT = '/dev/ttyUSB1'
BAUDRATE = 115200

ser = serial.Serial(PORT, BAUDRATE, timeout=1)
print(f"[EMULATOR] Listening on {PORT}...")

try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line == 'GET':
            voltage = round(random.uniform(11.0, 13.0), 2)
            current = round(random.uniform(0.0, 10.0), 2)
            temperature = round(random.uniform(20.0, 80.0), 2)

            response = f"VOLT:{voltage};CURR:{current};TEMP:{temperature}\n"
            ser.write(response.encode('utf-8'))

            print(f"[EMULATOR] Received: GET → Sent: {response.strip()}")
        elif line != '':
            print(f"[EMULATOR] Unknown command: {line}")

except KeyboardInterrupt:
    print("\n[EMULATOR] Stopped by user.")

finally:
    ser.close()
