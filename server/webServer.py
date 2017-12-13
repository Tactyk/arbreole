import tornado.httpserver
import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.gen
import os
import time

from tornado.options import define, options

static_path = os.path.join(os.path.dirname(__file__), '../static')

define("port", default=8080, help="run on the given port", type=int)

clients = []

class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('../views/index.html')

class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        print('New connection')
        clients.append(self)
        self.write_message("Server connection opened")

    def on_message(self, message):
        print('Server receiving from client: %s' % message)
        time.sleep(1)
        for client in clients:
            client.write_message('Server sending message: %s' % message)

    def on_close(self):
        print('Server connection closed')
        clients.remove(self)

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
