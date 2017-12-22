def send_all(hostname, clients):
    for client in clients:
        client.write_message(({
            "message": "Connection opened",
            "hostname": hostname,
            "type": "new_connection",
        }))
