import os
import configparser

import tornado.httpserver
import tornado.ioloop
from tornado.options import define, options

from indexHandler import IndexHandler
from webSocketHandler import WebSocketHandler

static_path = os.path.join(os.path.dirname(__file__), '../interface/static')
config_path = os.path.join(os.path.dirname(__file__), '../config')

config = configparser.ConfigParser()
config.read(config_path + '/config.ini')
server_ip = config['server']['ServerIP']
server_port = config['server']['ServerPort']

# Defining tornado's global options
define("port", default=server_port, help="Server run on the given port", type=int)
define("ip", default=server_ip, help="Server run on the given ip", type=int)

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
    print("Server listening on ip:", options.ip, "at port", options.port)

    options.parse_command_line()

    # Initialize Tornado's app
    app = tornado.web.Application(handlers, **settings)

    # Define Http tornado's server and listen on the given port
    httpServer = tornado.httpserver.HTTPServer(app)
    httpServer.listen(options.port)

    # Starts websocket listening
    mainLoop = tornado.ioloop.IOLoop.instance()
    mainLoop.start()
