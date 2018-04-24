import time
import configparser

import services.serialHandler as serialHandler
import services.serverSender as serverSender
import services.eventHandler as eventHandler
import services.dbHandler as dbHandler


config = configparser.ConfigParser()
config.read('../config/config.ini')


def transform_data_to_state(data):
    if data == "1":
        print("ACTIVE")
        return 'ACTIVE'
    if data == "0":
        print("INACTIVE")
        return 'INACTIVE'


def handle_serial_data(data):
    print("### NEW SERIAL DATA ###")
    print("DATA:", data)
    state = transform_data_to_state(data)
    if state is None:
        return
    current_time = time.time()

    if state == 'TERMINATED':
        print("RENEW:")
        current_state = dbHandler.get_state()
        msg = serialHandler.get_message_by_state(current_state)
        print("SERIAL MESSAGE:", msg)
        #serialHandler.write(msg)
    else:
        dbHandler.add_serial_signal(state, current_time)
        event = eventHandler.get_event_to_trigger(state, current_time)

        print("PRIOR EVENT", event)
        if event is not None:
            if eventHandler.should_trigger_event(event, current_time):
                new_state = update_state(event, current_time)
                serverSender.send(new_state['state'])
                msg = serialHandler.get_message_by_state(new_state)
                print("SERIAL MESSAGE:", msg)
                spot_msg = '<S,1,226,122,1,1000>'
                #serialHandler.write(msg)
                #serialHandler.write(spot_msg)


def pre_update():
    def decorated(func):
        def wrapper(*args, **kwargs):
            print("Exécution de la fonction %s." % func.__name__)
            event = args[0]
            current_time = args[1]

            state = dbHandler.get_state()
            print("OLD STATE", state)
            msg = {
                'id': dbHandler.STATE_KEY,
                'time': current_time,
                'state': event['name'],
                'previousTime': state['time'],
                'previousState': state['state'],
            }

            response = func(msg, current_time, **kwargs)
            print("Post-traitement.")
            return response
        return wrapper
    return decorated


@pre_update()
def update_state(msg, current_time):

    new_state = dbHandler.update_state(msg)

    print("NEW STATE", new_state)

    return new_state[0]
