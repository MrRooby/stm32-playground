import serial
import psutil
import struct

def open_serial_port(possible_ports, baud=115200, timeout=1):
    for port in possible_ports:
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

while True:
    temp = psutil.sensors_temperatures()
    cpu_temp = temp['legion_hwmon'][0].current
    byte_array_8 = struct.pack('f', cpu_temp)
    tx = bytes([int(cpu_temp)])
    ser.write(tx)
    print(tx.hex(' '))
