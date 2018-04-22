import json
import time

import services.dbHandler as dbHandler

events = json.load(open('events.json'))


def get_event_to_trigger(state, current_time):
    print("----------------------------------------")
    events = get_events_by_state(state)

    events_to_trigger = []

    for event in events:
        if could_trigger_event(event, current_time):
            events_to_trigger.append(event)

    return get_prior_event(events_to_trigger)


def should_trigger_event(event, current_time):
    state = dbHandler.get_state()

    if event['name'] == 'ACTIVE_0':
        if state['state'] == 'INACTIVE':
            print('>>>>SHOULD TRIGGER ACTIVE_0 <<<<')
            return True
        else:
            print('::::: IGNORING ACTIVE_0 ::::')
            return False

    if event['name'] == 'ACTIVE_1':
        if state['state'] == 'ACTIVE_0':
            print('>>>>SHOULD TRIGGER ACTIVE_1 <<<<')
            return True
        else:
            print('::::: CANNOT HAVE ACTIVE_1 after state ::::', state['state'])
            return False

    if event['name'] == 'ACTIVE_5':
        if state['state'] == 'ACTIVE_0':
            print('>>>>TRIGGER ACTIVE_5 <<<<')
            return True
        if state['state'] == 'ACTIVE_1':
            print('>>>>TRIGGER ACTIVE_5 after ACTIVE_1<<<<')
            return True
        if state['state'] == 'ACTIVE_5':
            time = current_time - float(state['time'])
            if time > 5:
                print('>>>>RENEW TRIGGER ACTIVE_5<<<<')
                return True
        else:
            print('::::: IGNORING ACTIVE_5 ::::')
            return False

    if event['name'] == 'INACTIVE':
        if state['state'] != 'INACTIVE':
            time = current_time - float(state['time'])
            if time > 5:
                print('>>>>TRIGGER INACTIVATION <<<<')
                return True
        return False


def get_events_by_state(state):

    return events[state]


def get_prior_event(events):
    if len(events) == 0:
        return None

    prior_event = events[0]

    for event in events:
        if event['priority'] > prior_event['priority']:
            prior_event = event

    return prior_event


def could_trigger_event(event, current_time):

    signals = dbHandler.get_last_signals(event['time'], current_time)

    signals_number = len(signals)

    if signals_number == 0:
        if event['name'] == 'ACTIVE_0':
            return True
        else:
            return False

    good_state_number = 0
    acceptance = False
    has_enough_good_state = False

    counter = 0

    if (current_time - event['time']) - signals[0]['time'] < 0:
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

