import tornado.httpserver
import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.gen
import os
import time
import configparser

from services import clientSender

from tornado.options import define, options

static_path = os.path.join(os.path.dirname(__file__), '../interface/static')
config_path = os.path.join(os.path.dirname(__file__), '../config')

config = configparser.ConfigParser()
config.read(config_path + '/config.ini')
server_ip = config['server']['ServerIP']
print(server_ip)

define("port", default=8080, help="run on the given port", type=int)

clients = []

class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('../interface/views/index.html', server_ip=server_ip)

class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        if 'Hostname' in self.request.headers:
            client_hostname = self.request.headers['Hostname']
            self.hostname = client_hostname
        else:
            client_hostname = 'interface'
            self.hostname = client_hostname
        print('New client connection with hostname: ' + client_hostname)
        clients.append(self)
        clientSender.send_all(client_hostname, clients)

    def on_message(self, message):
        print('Receiving from client: %s' % message)
        for client in clients:
            client.write_message(message)

    def on_close(self):
        print('Client connection closed')
        print(self.hostname)
        clients.remove(self)

    def on_ping(self, data):
        print('PING ===>' + self.hostname)

settings = {
    'debug': True,
    'static_path': static_path
}

handlers = [
    (r"/", IndexHandler),
    (r"/ws", WebSocketHandler)
]

if __name__ == '__main__':
    tornado.options.parse_command_line()
    app = tornado.web.Application(handlers, **settings)
    httpServer = tornado.httpserver.HTTPServer(app)
    httpServer.listen(options.port)
    print("Server listening on port:", options.port)
    mainLoop = tornado.ioloop.IOLoop.instance()
    mainLoop.start()
