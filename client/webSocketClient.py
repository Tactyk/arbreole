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

from services.serialSimulator import start_simulation
from tinydb import TinyDB, Query


config = configparser.ConfigParser()
config.read('../config/config.ini')

db = TinyDB('client.json')
db.purge_tables()

connect_to_arduino = True
client_simulation = False
serial_simulation = True

#=========================
#  code to ensure a cleaan exit

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


def handle_serial_data(data):
    print("### NEW SERIAL DATA ###")
    print("DATA:", data)
    state = transform_data_to_state(data)
    newState = update_state(state)
    analyseStateAndTriggerEvent(newState)


def pre_update():
    def decorated(func):
        def wrapper(*args, **kwargs):
            print("Exécution de la fonction %s." % func.__name__)
            print("ARG0", args[0])

            State = Query()
            res = db.search(State.id == hostname)
            currentTime = time.time()

            msg = {
                'id': hostname,
                'time': currentTime,
                'state': args[0]
            }

            if (len(res) == 0):
                msg['initialTime'] = currentTime
                msg['previousState'] = 'INACTIVE'
            else:
                msg['previousState'] = res[0]['state']

            response = func(msg, **kwargs)
            print("Post-traitement.")
            return response
        return wrapper
    return decorated

@pre_update()
def update_state(msg):

    print('UPDATE_STATE_MSG', msg)
    State = Query()
    db.upsert(msg, State.id == msg['id'])
    newState = db.search(State.id == hostname)

    print('DB STATE', db.all())

    return newState


def transform_data_to_state(data):
    if data == 1:
        return 'ACTIVE'
    if data == 0:
        return 'INACTIVE'

def analyseStateAndTriggerEvent(state):
    print('analyse STATE', state)
    state = state[0]
    time = float(state['time']) - float(state['initialTime'])

    if state['state'] == 'ACTIVE':
        if (time > 5):
            serverSender.send('ACTIVE_5')
            serial.
            print('MODULE ACTIVATION >5s :::', time)

        elif (time > 1):
            serverSender.send('ACTIVE_1')
            print('MODULE ACTIVATION >1s :::', time)
        elif (time == 0):
            serverSender.send('ACTIVE_0')
            print('MODULE ACTIVATED :::', time)
    if state['state'] == 'INACTIVE':
        serverSender.send('INACTIVE_0')
        print('MODULE INACTIVE:::', time)

def triggerLightEvent(event):


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
