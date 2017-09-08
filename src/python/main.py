import time
import rpiBle as rb
import rpiUdpServer as ru
import bluepy.btle as bt
import sys

"""
    Arduino ---(raw Temp val/0~1023/)--> Rpi --(temp val /ex: 35.5/) --> PC(Unity)
                handleNotification()             udpServer.send()

    Arduino <-- (Motor Command) -------- Rpi <--(Motor Cmd, Peltier Cmd) -- PC(Unity)
              bleClient.send()                   udpListener()

"""

#class BleListener(bt.DefaultDelegate):
#    def __init__(self):
#        bt.DefaultDelegate.__init__(self)
#                                                                            
#    def handleNotification(self,cHandle,data):
#        if cHandle==0x0025 :
#            print(data)
#            self.udpServer.send(data)
#    def addUdpServer(self,udpServer):
#        self.udpServer = udpServer

def BleListener(data,udpServer):
    udpServer.send(data)
                                                                                                                                                
def UdpListener(data,bleClient):
    print(data)
    bleClient.send(data)
    
    
# Main


udpServer = ru.RpiUdpServer('',7000,UdpListener)
#bleListener = BleListener()
#bleListener.addUdpServer(udpServer)
bleClient = rb.RpiBle("a4:d5:78:0d:07:a2",BleListener)
bleClient.addUdpServer(udpServer)
udpServer.addBleClient(bleClient)
udpServer.daemon = True
bleClient.daemon = True
udpServer.start()
bleClient.start()

while True:
    #time.sleep(1000)
    data = raw_input()
    bleClient.send(data)
