import socket
import threading
import sys

class RpiUdpServer(threading.Thread):
    def __init__(self,HOST,PORT,listener):
        threading.Thread.__init__(self)
        self.HOST = HOST
        self.PORT = PORT
        self.BUFSIZE=1024
        ADDR = (HOST,PORT)
        self.udpServer = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.udpServer.bind(ADDR)
        self.listener = listener
        self.ClientAddr = None
    def addBleClient(self,bleCli):
        self.bleClient = bleCli

    def run(self):
        print("Waiting for messages")
        data, addr = self.udpServer.recvfrom(self.BUFSIZE)
        self.ClientAddr = addr
        while True:

            data, addr = self.udpServer.recvfrom(self.BUFSIZE)
            self.listener(data,self.bleClient)
                     
    def send(self,message):
        if self.ClientAddr is not None:
            self.udpServer.sendto(message,self.ClientAddr)
            print("send to PC : %s" % message)
        else : 
            print("Udp Server not Ready!")

    def __del__(self):
        self.udpServer.close()


if __name__=="__main__":
    def MyListener(data):
        print(data)
        udpServer.send("Hi Unity!")


    HOST = ''
    PORT = 7000
    udpServer = RpiUdpServer(HOST,PORT,MyListener)
    udpServer.daemon=True
    udpServer.start()
    import time
    while True:
        time.sleep(1000)

