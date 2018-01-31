import serial


class SerialWriter:

    def __init__(self):
        self.conn = serial.Serial('/dev/ttyACM0', 9600)

    def write(self, message):
        self.conn.write(message.encode('utf-8'))
