import time
import os
import services.serverSender as serverSender

def start_simulation(self):

    serverSender.send('EVENT_STATE_1')
    time.sleep(1)
    serverSender.send('EVENT_STATE_2')
    time.sleep(1)
    serverSender.send('EVENT_STATE_3')
