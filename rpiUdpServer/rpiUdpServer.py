from socket import *
import threading
from time import ctime
import sys

class RpiUdpServer(threading.Thread):
    def __init__(self,HOST,PORT,listener):
        threading.Thread.__init__(self)
        self.HOST = HOST
        self.PORT = PORT
        self.BUFSIZE=1024
        ADDR = (HOST,PORT)
        self.listener = listener
        self.udpServer = socket(AF_INET,SOCK_DGRAM)
        self.udpServer.bind(ADDR)
        
    def run(self):
        print("Waiting for messages")
        
        data, addr = self.udpServer.recvfrom(self.BUFSIZE)
        self.ClientAddr = addr
         
        while True:

            data, addr = self.udpServer.recvfrom(self.BUFSIZE)
            self.listener(data)
                     
    def send(self,message):
        self.udpServer.sendto('[%s] %s' % (ctime(),message),self.ClientAddr)

    def __del__(self):
        self.udpServer.close()


        
