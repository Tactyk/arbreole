import websocket
import time
import configparser
import os
import json

from services.serialWriter import SerialWriter
from services.serverSender import ServerSender
from services.simulator import Simulator

config = configparser.ConfigParser()
config.read('../config/config.ini')

connect_to_arduino = True
simulation = False

if connect_to_arduino:
    global serialWriter
    # Connect to Arduino via serial
    try:
        serialWriter = SerialWriter()

        print("Arduino connected")
    except:
        connect_to_arduino = False
        print("Arduino not connected")


# Input message handling from server
def on_message(ws, message):
    print("Receiving message: " + message)
    if connect_to_arduino:
        serialWriter.write(message)


def on_error(ws, error):
    print("### ERROR ###")
    print(error)


def on_close(ws):
    print("### CLOSED ###")


def on_open(ws):
    print("### OPEN ###")
    sender = ServerSender(ws)
    if simulation is True:
        simulator = Simulator(sender)
        simulator.start_simulation()


address = config['server']['ServerIP'] + ':' + config['server']['ServerPort']
print(address)
hostname = os.uname()[1]

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
