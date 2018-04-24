import json
import os
import time
import random

colorsByPhases = json.load(open('colors.json'))
functionsByPhases = json.load(open('functions.json'))

from tinydb import TinyDB, Query

hostname = os.uname()[1]

STATE_KEY = 'state'


def initialize_database():
    global db
    db = TinyDB('client.json')
    global serial_events
    serial_events = TinyDB('serial_signals.json')
    db.purge_tables()
    serial_events.purge_tables()


def initialize_colors():
    for functionsByPhase in functionsByPhases:
            db.insert(functionsByPhase)


def add_serial_signal(state, current_time):

    serial_signal = {
        'state': state,
        'time': current_time,
    }

    serial_events.insert(serial_signal)
    query = Query()
    serial_events.remove(query.time < current_time - 10)


def initialize_functions():
    for colorsByPhase in colorsByPhases:
            db.insert(colorsByPhase)


def initialize_state():
    global db
    db.insert(
        {
            "id": STATE_KEY,
            "phase": "P0",
            "state": "INACTIVE",
            'previousState': "INACTIVE",
            'previousTime': time.time(),
            'time': time.time(),
        }
    )


def get_state():
    query = Query()
    state = db.search(query.id == STATE_KEY)

    return state[0]


def extract_time(json):
    try:
        # Also convert to int since update_time will be string.  When comparing
        # strings, "10" is smaller than "2".
        print("JSON TIME", json['time'])
        return int(json['time'])
    except KeyError:
        return 0


def get_last_signals(time, current_time):
    signal = Query()
    signals = serial_events.search(signal.time >= (current_time - time - 1))

    signals_before = []
    signals_after = []

    for signal in signals:
        if signal['time'] < (current_time - time):
            signals_before.append(signal)
        else:
            signals_after.append(signal)

    signals_before = sorted(signals_before, key=lambda i: i['time'], reverse=True)
    signals_after = sorted(signals_after, key=lambda i: i['time'])

    if len(signals_before) > 0:
        signal_before = signals_before.pop(0)
    else:
        signal_before = []

    if len(signals_before) > 0:
        if time == 0:
            return [signal_before]

        signals_after.insert(0, signal_before)

    return signals_after


def update_state(msg):
    global hostname
    state = Query()
    print(STATE_KEY)
    db.update(msg, state.id == STATE_KEY)

    return db.search(state.id == STATE_KEY)


def get_colors_by_event_for_phase(event, phase, number):
    colors = get_colors_by_phase(phase)
    status = event.split('_')[0]

    print("colors colors before", colors[status])
    if len(colors[status]) == 0:
        reinit_phase_colors(phase, status)
        colors = get_colors_by_phase(phase)

        status = event.split('_')[0]

    values = []
    for i in range(0, number):
        values.append(colors[status].pop())

    print("colors colors after", colors[status])

    phase_query = Query()
    db.update(colors, phase_query.id == 'colors-' + phase)

    return values


def get_colors_by_phase(phaseId):
    phase = Query()
    result = db.search(phase.id == 'colors-' + phaseId)

    return result[0]


def reinit_phase_colors(phaseId, status):
    dbColors = get_colors_by_phase(phaseId)
    print("DBCOLORs", dbColors)

    for colorsByPhase in colorsByPhases:
        if colorsByPhase['id'] == 'colors-' + phaseId:
            print(colorsByPhase)

            dbColors[status] = colorsByPhase[status]
            phase_query = Query()
            db.update(dbColors, phase_query.id == colorsByPhase['id'])
            print(colorsByPhase['id'])

            yoyo = Query()
            kioo = db.search(yoyo.id == colorsByPhase['id'])
            print(kioo)


def get_led_function_by_event_for_phase(event, phase):

    functions = get_led_functions_by_phase_and_event('L',event, phase)

    return random.choice(functions)


def get_led_functions_by_phase_and_event(type, event, phaseId):
    phase = Query()
    result = db.search(phase.id == 'functions-'+phaseId)
    functions_by_event = result[0]

    return functions_by_event[event][type]


