import json
import time

class Simulator:

    def __init__(self, socket):
        self.socket = socket

    def start_simulation(self):
        message_serialized = json.dumps({'foo': 'hello'})
        self.socket.send(message_serialized)
        time.sleep(5)
        message_serialized_2 = json.dumps({'bar': 'world'})
        self.socket.send(message_serialized_2)