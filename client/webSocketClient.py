import websocket
import _thread
import time
import configparser
import os
import subprocess
import serial

config = configparser.ConfigParser()
config.read('../config/config.ini')

try:
    dev = subprocess.check_output('ls /dev/ttyACM0', shell=True)
    print(dev)
    ser = serial.Serial('/dev/ttyACM0', 9600)
    print("Arduino Connected")
except:
    print("Arduino not connected")

def on_message(ws, message):
    print("Receiving message: " + message)
    global ser
    if message == '1':
        ser.write('1'.encode('utf-8'))
        time.sleep(1)
    elif message == '2':
        ser.write('2'.encode('utf-8'))
        time.sleep(1)
    elif message == '3':
        ser.write('3'.encode('utf-8'))
        time.sleep(1)
    elif message == '4':
        ser.write('4'.encode('utf-8'))
        time.sleep(1)
    elif message == '5':
        ser.write('5'.encode('utf-8'))
        time.sleep(1)
    else:
        ser.write(message.encode('utf-8'))
        time.sleep(1)

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### CLOSED ###")

def on_open(ws):
    print("### OPEN ###")

address = config['server']['ServerIP'] + ':' + config['server']['ServerPort']
print(address)
hostname = os.uname()[1]

print(address)

if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://" + address + "/ws",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close,
                              header = { 'hostname' : hostname })
    ws.on_open = on_open
    ws.run_forever(ping_interval = 10)