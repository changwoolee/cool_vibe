from rpiUdpServer import RpiUdpServer

def MyListener(data):
    print(data)
    udpServer.send("Hi Unity!")



HOST = ''
PORT = 7000


udpServer = RpiUdpServer(HOST,PORT,MyListener)
udpServer.daemon=True
udpServer.start()

while True:
    pass

