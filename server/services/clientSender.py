class ClientSender:

    @staticmethod
    def send_all(hostname, type, clients, msg=None):
        for client in clients:
            if client.hostname != hostname:
                client.write_message({
                    'hostname': hostname,
                    'message': msg,
                    'type': type,
                })
