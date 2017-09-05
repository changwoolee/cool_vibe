import time
from rpiBle import *
from rpiUdpServer import *
from bluepy.btle import DefaultDelegate
import sys

"""
    Arduino ---(raw Temp val/0~1023/)--> Rpi --(temp val /ex: 35.5/) --> PC(Unity)
                handleNotification()             udpServer.send()

    Arduino <-- (Motor Command) -------- Rpi <--(Motor Cmd, Peltier Cmd) -- PC(Unity)
              bleClient.send()                   udpListener()

"""
class BleListener(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)
                                                                            
    def handleNotification(self,cHandle,data):
        if cHandle==0x0025 :
            print(data)
            self.udpServer.send(data)
    def addUdpServer(self,udpServer):
        self.udpServer = udpServer


                                                                                                                                                
def UdpListener(data,BleClient):
    print(data)
    BleClient.send(data)
    
    
# Main


udpServer = RpiUdpServer('',7000,UdpListener)
bleListener = BleListener()
bleListener.addUdpServer(udpServer)
bleClient = RpiBle("a4:d5:78:0d:07:a2",bleListener)
udpServer.addBleClient(bleClient)
udpServer.daemon = True
bleClient.daemon = True
udpServer.start()
bleClient.start()

while True:
    time.sleep(1000)
