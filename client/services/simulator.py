import time
import os

from .serverSender import ServerSender

class Simulator:

    def __init__(self, sender):
        self.sender = sender

    def start_simulation(self):

        self.sender.send('EVENT_STATE_1')
        time.sleep(1)
        self.sender.send('EVENT_STATE_2')
        time.sleep(1)
        self.sender.send('EVENT_STATE_3')
