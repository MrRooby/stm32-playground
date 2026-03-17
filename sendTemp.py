import serial
import psutil
import struct
import time
import os

def open_serial_port(possible_ports, baud=115200, timeout=1):
    for port in possible_ports:
        if os.path.exists(port):
            ser = serial.Serial(port, baudrate=baud, timeout=timeout)
            print(f"Connected to {port}")
            return ser
    print("Error!! No ports available")
    return None

ports = [
    '/dev/ttyACM1',
    '/dev/ttyACM0',
]

ser = open_serial_port(ports, 115200)
print("Serial opened")

while True:
    temp = psutil.sensors_temperatures()
    cpu_temp = temp['legion_hwmon'][0].current
    tx1 = bytes([int(cpu_temp)])

    cpu_usage = psutil.cpu_percent()
    tx2 = bytes([int(cpu_usage)])

    ram_usage = psutil.virtual_memory().percent
    tx3 = bytes([int(ram_usage)])
    tx4 = bytes([0xFF])

    ser.write(tx1 + tx2 + tx3)
    time.sleep(0.01)
    ser.write(tx3)
    ser.write(tx4)

    print("TEMP: " + str(cpu_temp))
    print("CPU: " + str(cpu_usage))
    print("RAM: " + str(ram_usage))
    time.sleep(0.3)

