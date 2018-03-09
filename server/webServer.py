import tornado.httpserver
import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.gen
import os
import configparser
import json
import time

from tinydb import TinyDB, Query
from services.clientSender import ClientSender
from tornado.options import define, options

static_path = os.path.join(os.path.dirname(__file__), '../interface/static')
config_path = os.path.join(os.path.dirname(__file__), '../config')

config = configparser.ConfigParser()
config.read(config_path + '/config.ini')
server_ip = config['server']['ServerIP']
server_port = config['server']['ServerPort']
db = TinyDB('db.json')
db.purge_tables()

# Defining tornado's global options
define("port", default=server_port, help="Server run on the given port", type=int)
define("ip", default=server_ip, help="Server run on the given ip", type=int)

clients = []

# Webserver Handler
class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('../interface/views/index.html', server_ip=server_ip)

# Socket Handler
class WebSocketHandler(tornado.websocket.WebSocketHandler):

    def open(self):
        client_sender = ClientSender()
        if 'Hostname' in self.request.headers:
            client_hostname = self.request.headers['Hostname']
            self.hostname = client_hostname
        else:
            client_hostname = 'interface'
            self.hostname = client_hostname
        print('New client connection with hostname: ' + client_hostname)
        clients.append(self)
        client_sender.send_all(client_hostname, clients)

    def on_message(self, message):
        print('Receiving from client: %s' % message)
        message_decoded = json.loads(message)
        print("DB STATE", db.all())
        if 'type' in message_decoded:
            if 'rpi_client' == message_decoded['type']:
                update_table(message_decoded)
                print("DB STATE", db.all())

#        for client in clients:
#            client.write_message(message)

    def on_close(self):
        print('Client connection closed')
        print(self.hostname)
        clients.remove(self)

    def on_ping(self, data):
        print('PING ===>' + self.hostname)


def pre_update():
    def decorated(func):
        def wrapper(*args, **kwargs):
            print("Exécution de la fonction %s." % func.__name__)
            for arg in args:
                print(arg)

            response = func(*args, **kwargs)
            print ("Post-traitement.")
            return response
        return wrapper
    return decorated


@pre_update()
def update_table(msg):
    Rpi = Query()
    msg['time'] = time.time()
    db.upsert(msg, Rpi.id == msg['id'])

settings = {
    'debug': True,
    'static_path': static_path
}

# Define handler for webserver and sockets
handlers = [
    (r"/", IndexHandler),
    (r"/ws", WebSocketHandler)
]

if __name__ == '__main__':
    print("Server listening on ip:", options.port, "at port", options.ip)

    options.parse_command_line()

    # Initialize Tornado's app
    app = tornado.web.Application(handlers, **settings)

    # Define Http tornado's server and listen on the given port
    httpServer = tornado.httpserver.HTTPServer(app)
    httpServer.listen(options.port)

    # Starts websocket listening
    mainLoop = tornado.ioloop.IOLoop.instance()
    mainLoop.start()
