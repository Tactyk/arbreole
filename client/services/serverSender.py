import json
import os

class ServerSender:

    def __init__(self, socket):
        self.socket = socket

    def send(self, event):
        json_message = {
            'id': os.uname()[1],
            'type': 'rpi_client',
            'event': event,
        }
        msg = json.dumps(json_message)

        self.socket.send(msg)
