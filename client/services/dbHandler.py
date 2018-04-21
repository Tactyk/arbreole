import json
import os
import time

colorsByPhases = json.load(open('colors.json'))
functionsByPhases = json.load(open('functions.json'))

from tinydb import TinyDB, Query

hostname = os.uname()[1]

STATE_KEY = 'state'


def initialize_database():
    global db
    db = TinyDB('client.json')
    db.purge_tables()


def initialize_colors():
    for functionsByPhase in functionsByPhases:
            db.insert(functionsByPhase)


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


def update_state(msg):
    global hostname
    state = Query()
    print(STATE_KEY)
    db.update(msg, state.id == STATE_KEY)

    return db.search(state.id == STATE_KEY)


def get_color_by_event_for_phase(event, phase):
    colors = get_colors_by_phase(phase)
    status = event.split('_')[0]
    print("STATUS", status)

    print("colors colors before", colors[status])
    if (len(colors[status]) == 0):
        reinit_phase_colors(phase, status)

    value = colors[status].pop()

    print("colors colors after", colors[status])

    phase_query = Query()
    db.update(colors, phase_query.id == phase)

    query = Query()
    result = db.search(query.id == phase)

    print("RESULT COLOR PHASE", result)

    return value


def get_colors_by_phase(phaseId):
    phase = Query()
    result = db.search(phase.id == 'colors-'+phaseId)

    return result[0]


def reinit_phase_colors(phaseId, status):
    dbColors = get_colors_by_phase(phaseId)

    for colorsByPhase in colorsByPhases:
        if colorsByPhase['id'] == phaseId:
            dbColors[status] = colorsByPhases[status]
            phase_query = Query()
            db.update(dbColors, phase_query.id == id)


def get_led_function_by_event_for_phase(event, phase):

    functions = get_led_functions_by_phase_and_event('L',event, phase)

    return functions[0]


def get_led_functions_by_phase_and_event(type, event, phaseId):
    phase = Query()
    result = db.search(phase.id == 'functions-'+phaseId)
    functions_by_event = result[0]

    return functions_by_event[event][type]

