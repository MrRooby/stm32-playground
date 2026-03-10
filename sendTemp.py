import serial
import datetime
import time
import psutil
import struct

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

while True:
    temp = psutil.sensors_temperatures()
    cpu_temp = temp['legion_hwmon'][0].current
    byte_array_8 = struct.pack('f', cpu_temp)
    tx = bytes([int(cpu_temp)])
    ser.write(tx)
    print(tx.hex(' '))
    

