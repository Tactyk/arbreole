import json
import time

from tinydb import TinyDB, Query

data = json.load(open('colors.json'))


def init_database():
    global db
    db = TinyDB('server.json')
    db.purge_tables()
    colors = json.load(open('colors.json'))

    for color in colors:
        db.insert(color)

    global_state = {
        'id': 'global_state',
        'status': 'INACTIVE',
    }

    db.insert(global_state)


def init_client_state(hostname):
    initial_state = {
        'id': hostname,
        'type': 'rpi_client',
        'event': 'CONNECTED',
        'time': time.time()
    }

    db.insert(initial_state)


def get_global_status():
    query = Query()

    return db.search(query.id == 'global_status')


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
    db.upsert(colors, phase_query.id == phase)

    yoyo = Query()
    result = db.search(yoyo.id == phase)

    print("RESULT COLOR PHASE", result)

    return value


def get_colors_by_phase(phaseId):
    phase = Query()
    result = db.search(phase.id == phaseId)

    return result[0]

def get_all():
    return db.all()


def upsert(msg, id):
    module = Query()
    db.upsert(msg, module.id == id)


def reinit_phase_colors(phaseId, status):
    dbColors = get_colors_by_phase(phaseId)

    for dataColors in data:
        if dataColors['id'] == phaseId:
            dbColors[status] = dataColors[status]
            phase_query = Query()
            db.upsert(dbColors, phase_query.id == id)
