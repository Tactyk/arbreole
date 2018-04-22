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
serial_simulation = True

dbHandler.initialize_database()
dbHandler.initialize_state()
dbHandler.initialize_colors()
dbHandler.initialize_functions()
print("WAITING FOR ACTIVATION")

res = dbHandler.get_led_function_by_event_for_phase('INACTIVE', 'P0')
col = dbHandler.get_colors_by_event_for_phase('INACTIVE', 'P0', 2)
print(res)
print(col)
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


def transform_data_to_state(data):
    if data == 1:
        return 'ACTIVE'
    if data == 0:
        return 'INACTIVE'


def handle_serial_data(data):
    print("### NEW SERIAL DATA ###")
    print("DATA:", data)
    state = transform_data_to_state(data)
    current_time = time.time()
    dbHandler.add_serial_signal(state, current_time)
    events = eventHandler.get_event_to_trigger(state, current_time)


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

            if should_init_time(msg):
                msg['time'] = currentTime
                msg['previousTime'] = currentTime

            # si il est inactif depuis plus de 5 sec, reinitialisation du temps
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

    return new_state@pre_update()


def should_init_time(state):
    return is_freshly_activate(state) or is_freshly_inactivate(state)


def is_freshly_activate(state):
    if state['previousState'] == 'INACTIVE'and state['state'] == 'ACTIVE':
        return True
    else:
        return False


def is_freshly_inactivate(state):
    if state['previousState'] == 'ACTIVE'and state['state'] == 'INACTIVE':
        return True
    else:
        return False


def is_inactivated(state):
    time = float(state['time']) - float(state['previousTime'])
    if state['state'] == 'INACTIVE'and state['previousState'] == 'INACTIVE' and time > 5:
        return True
    else:
        return False


def is_strongly_activated(state):
    time = float(state['time']) - float(state['previousTime'])
    if state['state'] == 'ACTIVE'and state['previousState'] == 'ACTIVE' and time > 5:
        return True
    else:
        return False



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
