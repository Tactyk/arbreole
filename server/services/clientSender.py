def send_all(msg, clients):
    for client in clients:
        client.write_message(msg)
