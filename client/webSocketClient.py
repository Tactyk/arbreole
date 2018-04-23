import websocket
import time
import configparser
import os
import json
import atexit

import services.checkForArduinoData as checkForArduinoData
import services.serialHandler as serialHandler
import services.serverSender as serverSender
import services.clientSimulator as clientSimulator
import services.eventHandler as eventHandler
import services.dbHandler as dbHandler

from services.serialSimulator import start_simulation
from tinydb import TinyDB, Query


config = configparser.ConfigParser()
config.read('../config/config.ini')

connect_to_arduino = True
client_simulation = False
serial_simulation = False

dbHandler.initialize_database()
dbHandler.initialize_state()
dbHandler.initialize_state()
dbHandler.initialize_colors()
dbHandler.initialize_functions()
print("WAITING FOR ACTIVATION")

#=========================
#  code to ensure a clean exit

def exit_handler():
    print("My application is ending!")
    checkForArduinoData.stopListening()
    serialHandler.closeSerial()

atexit.register(exit_handler)

#===========================

if connect_to_arduino:
    # Connect to Arduino via serial
    try:
        serialHandler.setupSerial()
        print("Arduino connected")
        # Start the listening thread
        checkForArduinoData.listenForData()
    except:
        connect_to_arduino = False
        print("Arduino not connected")


# Input message handling from server
def on_message(ws, message):
    print("Receiving message: " + message)
    if connect_to_arduino:
        serialHandler.write(message)


def on_error(ws, error):
    print("### ERROR ###")
    print(error)


def on_close(ws):
    print("### CLOSED ###")


def on_open(ws):
    print("### OPEN ###")
    serverSender.setup(ws)
    if client_simulation is True:
        clientSimulator.start_simulation()

    if serial_simulation is True:
        start_simulation()


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
