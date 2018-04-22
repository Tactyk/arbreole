import json
import time

import services.dbHandler as dbHandler

events = json.load(open('events.json'))


def get_event_to_trigger(state):
    print("----------------------------------------")
    events = get_events_by_state(state)

    events_to_trigger = []

    current_time = time.time()
    for event in events:
        if could_trigger_event(event, current_time):
            events_to_trigger.append(event)

    print("EVENTS TO TRIGGER", events_to_trigger)

    # new_event = get_event_to_trigger(events_to_trigger)
    #
    # if should_trigger_event(new_event):
    #     trigger_event()
    #     update_state(new_event)


def get_events_by_state(state):
    return events[state]


def could_trigger_event(event, current_time):

    signals = dbHandler.get_last_signals(event['time'], current_time)
    print("Check event", event['name'])
    print("current time", current_time)

    signals_number = len(signals)

    if signals_number == 0:
        print("4 - NO SIGNALS ...")
        return False

    good_state_number = 0
    acceptance = False
    has_enough_good_state = False

    counter = 0

    if (current_time - event['time']) - signals[0]['time'] < 0:
        print("IGNORING SIGNAL")
        return False

    for signal in signals:
        if signal['state'] == event['state']:
            good_state_number = good_state_number + 1
            var1 = 1 - ((signals_number - counter) / signals_number)
            var2 = float(event['acceptance'])
            if var1 >= var2:
                acceptance = True
        counter = counter + 1

    if good_state_number/signals_number >= event['threshold']/100:
        has_enough_good_state = True

    return acceptance and has_enough_good_state

