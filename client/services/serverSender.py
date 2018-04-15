import json
import os

def setup(ws):
    global socket
    socket = ws

def send(event):
    global socket
    json_message = {
        'id': os.uname()[1],
        'type': 'rpi_client',
        'event': event,
    }
    msg = json.dumps(json_message)
    socket.send(msg)
