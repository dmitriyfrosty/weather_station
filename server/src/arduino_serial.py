#!/usr/bin/env python3

import serial
if __name__ == '__main__':
    ser = serial.Serial('/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0', 9600, timeout=1)
    ser.flush()
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('CP866').rstrip()
            print(line)