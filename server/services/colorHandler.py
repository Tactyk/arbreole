import json

data = json.load(open('data.json'))

def get_color_by_event_for_phase(event, phase, number):
    data[phase][]