import serial
import subprocess


class SerialWriter:

    def __init__(self):
        dev = subprocess.check_output('ls /dev/ttyACM*', shell=True)
        print("Arduino Detected: " + dev.decode('utf-8').strip())

        self.conn = serial.Serial(dev.decode('utf-8').strip(), 9600)

    def write(self, message):
        self.conn.write(message.encode('utf-8'))
