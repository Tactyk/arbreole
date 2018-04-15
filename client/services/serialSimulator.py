import time

def start_simulation():
    import webSocketClient

    # import insde function to avoid circular reference
#    from webSocketClient import handle_serial_data
    webSocketClient.handle_serial_data(1)
    time.sleep(0.3)
    webSocketClient.handle_serial_data(1)
    time.sleep(0.3)
    webSocketClient.handle_serial_data(1)
    time.sleep(1)
    webSocketClient.handle_serial_data(1)
    time.sleep(4)
    webSocketClient.handle_serial_data(1)
    time.sleep(0.3)
    webSocketClient.handle_serial_data(0)
    time.sleep(0.3)
