class ServerSender:

    def __init__(self, socket):
        self.socket = socket

    def send(self, obj):
        self.socket.send(obj)