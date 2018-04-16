import json
import os


from tinydb import TinyDB, Query

hostname = os.uname()[1]

def initialize_database():
    global db
    db = TinyDB('client.json')
    db.purge_tables()


def initialize_state():
    global db
    db.insert(
        {
            "id": 'module_state',
            "phase": "P0",
            "state": "INACTIVATED"
        }
    )


def find_state_by_id(hostname):
    state = Query()
    return db.search(state.id == hostname)


def upsert(data, id):
    module = Query()
    db.upsert(data, module.id == id)


def update_state(msg):
    global hostname
    state = Query()
    db.upsert(msg, state.id == msg['id'])

    return db.search(state.id == hostname)