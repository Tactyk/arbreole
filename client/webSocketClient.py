import websocket
import time
import configparser
import os
import serial

from services.serialWriter import SerialWriter

config = configparser.ConfigParser()
config.read('./config/config.ini')

debug = True

if not debug:
    global serialWriter
    # Connect to Arduino via serial
    try:
        serialWriter = SerialWriter()
    except:
        print("Arduino not connected")


# Input message handling from server
def on_message(ws, message):
    print("Receiving message: " + message)
    if not debug:
        serialWriter.write(message)


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
    ws = websocket.WebSocketApp(
        "ws://" + address + "/ws",
        on_message=on_message,
        on_error=on_error,
        on_close=on_close,
        header={'hostname': hostname}
    )
    ws.on_open = on_open
    ws.run_forever()