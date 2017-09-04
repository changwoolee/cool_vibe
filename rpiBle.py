from bluepy.btle import Peripheral, DefaultDelegate
import threading
import sys

class RpiBle(threading.Thread):
    def __init__(self,MAC,Handler):
        threading.Thread.__init__(self)

        self.hand = Peripheral(MAC)
        self.hand.withDelegate(Handler())
        
        characteristics = self.hand.getCharacteristics(0x0024,0x0026,"0000dfb1-0000-1000-8000-00805f9b34fb")
    
        self.wChar = characteristics[0]

        self.nowSend = False
        self.sendData = "initial" 
    
    
    def run(self):
        while True:
            if self.nowSend:
                self.wChar.write(self.sendData)
                self.nowSend=False

            if self.hand.waitForNotifications(1.0):
                continue

    def send(self,sdata):
        self.sendData=sdata
        self.nowSend=True
        #self.wChar.write(sdata)

    def __del__(self):
        self.hand.disconnect()


def lHandListener(data):
    print(data)
    return data


class NotifyHandler(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self,cHandle,data):
        if cHandle==0x0025:
            print(str(data))


if __name__=="__main__":
    lhand = RpiBle("a4:d5:78:0d:07:a2",NotifyHandler)
    lhand.daemon=True
    lhand.start()
    
    while True:
        data=raw_input()
        lhand.send(data)

        




