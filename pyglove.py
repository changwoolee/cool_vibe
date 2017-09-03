from bluepy.btle import Peripheral, DefaultDelegate

# rw Handle : Characteristic for Read & Write
rwHandle = 0x0025


# NotifyHandler(DefaultDelegate)
#   handleNotification is called if arduino sends a message

class NotifyHandler(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self,cHandle,data):
        if cHandle==rwHandle:
            print(data)


leftHand = Peripheral("a4:d5:78:0d:07:a2")
#rightHand = btle.Peripheral(" MAC Addr Here ")"

#characteristics = btle.getCharacteristics(Handle)

leftHand.withDelegate(NotifyHandler())


# Main Function

while True:
    leftHand.writeCharacteristic(rwHandle,"1234")
    if leftHand.waitForNotifications(1.0):
        continue

    print("Waiting...")

