import json
import time
import tornado.websocket
import services.dbHandler as database

database.init_database()

clients = []


# Socket Handler
class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        if 'Hostname' in self.request.headers:
            client_hostname = self.request.headers['Hostname']
        else:
            client_hostname = 'interface'
        self.hostname = client_hostname
        database.init_client_state(client_hostname)
        print('New client connection with hostname: ' + client_hostname)
        self.send_all('new_connection')
        clients.append(self)

    def on_message(self, message):
        global database
        global_status = database.get_global_status()
        if global_status == 'INACTIVE':
            print('Arbreole is inactive -> Ignoring message: ', message)
            return

        message_decoded = json.loads(message)
        print('Receiving from client: %s' % message_decoded)
        if 'type' in message_decoded:
            if 'rpi_client' == message_decoded['type']:
                update_table(message_decoded)
                print("DB STATE", database.get_all())
            if 'interface' == message_decoded['type']:
                self.send_all("command", True, message_decoded['message'])

    def on_close(self):
        print('Client connection closed')
        print(self.hostname)
        clients.remove(self)

    def send_all(self, type, include_self=False, msg=None):
        for client in clients:
            if client.hostname != self.hostname or include_self:
                client.write_message({
                    'hostname': self.hostname,
                    'message': msg,
                    'type': type,
                })


def pre_update():
    def decorated(func):
        def wrapper(*args, **kwargs):
            print("Exécution de la fonction %s." % func.__name__)
            response = func(*args, **kwargs)
            print("Post-traitement.")
            analyse_db()
            return response
        return wrapper
    return decorated


@pre_update()
def update_table(msg):
    msg['time'] = time.time()
    global database
    database.upsert(msg, msg['id'])


def analyse_db():
    results = database.get_all()
    print('post Request::DB STATE', results)
