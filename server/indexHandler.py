import tornado.web
from tornado.options import options

# Webserver Handler
class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        address = options.ip + ':' + options.port
        self.render('../interface/views/index.html', address=address)
