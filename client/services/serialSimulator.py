import time

def start_simulation():
    import services.dataHandler as dataHandler

    # import insde function to avoid circular reference
    # First t=0
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    # 5 -> t= 0.5
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    # 5 -> t= 1
    print("SHOULD ACTIVE_1")

    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)

    print("SHOULD ACTIVE_2")

    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)

    print("SHOULD ACTIVE_3")

    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)

    print("SHOULD ACTIVE_4")

    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)

    print("SHOULD ACTIVE_5")

    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    simulate_active_1_second()
    simulate_active_1_second()
    simulate_active_1_second()
    simulate_inactive_1_second()
    simulate_inactive_1_second()
    simulate_inactive_1_second()
    simulate_inactive_1_second()
    simulate_inactive_1_second()
    simulate_active_1_second()
    simulate_active_1_second()

def simulate_active_1_second():
    import dataHandler
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)
    time.sleep(0.1)
    dataHandler.handle_serial_data(1)

def simulate_inactive_1_second():
    import dataHandler
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)
    time.sleep(0.1)
    dataHandler.handle_serial_data(0)