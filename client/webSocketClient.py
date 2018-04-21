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
import services.dbHandler as dbHandler

from services.serialSimulator import start_simulation
from tinydb import TinyDB, Query


config = configparser.ConfigParser()
config.read('../config/config.ini')

connect_to_arduino = True
client_simulation = False
serial_simulation = True

dbHandler.initialize_database()
dbHandler.initialize_state()
dbHandler.initialize_colors()
dbHandler.initialize_functions()
res = dbHandler.get_led_function_by_event_for_phase('INACTIVE', 'P0')
print(res)
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


def handle_serial_data(data):
    print("### NEW SERIAL DATA ###")
    print("DATA:", data)
    state = transform_data_to_state(data)
    newState = update_state(state)
    analyse_state_and_trigger_event(newState)


def pre_update():
    def decorated(func):
        def wrapper(*args, **kwargs):
            print("Exécution de la fonction %s." % func.__name__)
            print("ARG0", args[0])

            res = dbHandler.get_state()
            currentTime = time.time()

            msg = {
                'id': dbHandler.STATE_KEY,
                'time': currentTime,
                'state': args[0],
                'previousTime': res['time'],
                'previousState': res['state'],
            }

            response = func(msg, **kwargs)
            print("Post-traitement.")
            return response
        return wrapper
    return decorated

@pre_update()
def update_state(msg):

    print('UPDATE_STATE_MSG', msg)
    new_state = dbHandler.update_state(msg)

    print('STATE UPDATED', new_state)

    return new_state


def transform_data_to_state(data):
    if data == 1:
        return 'ACTIVE'
    if data == 0:
        return 'INACTIVE'

def analyse_state_and_trigger_event(state):
    print('analyse STATE', state)
    state = state[0]
    time = float(state['time']) - float(state['previousTime'])

    if state['state'] == 'ACTIVE':
        if (time > 5):
            serverSender.send('ACTIVE_5')
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
