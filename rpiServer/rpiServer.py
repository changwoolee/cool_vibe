import socket, threading
import sys
import rpiServerThread

class RpiServer(threading.Thread):
    def __init__(self, HOST, PORT):
        threading.Thread.__init__(self)

        self.HOST = HOST
        self.PORT = PORT

        self.serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.serverSocket.bind((self.HOST, self.PORT))
        self.serverSocket.listen(1)

    def run(self):
        try:
            print("Waiting the Client..")
            connection, clientAddress = self.serverSocket.accept()
            self.client = connection
            print("Client Connected,'{0}'".format(clientAddress))
            
            self.subThread = rpiServerThread.RpiServerThread(connection,clientAddress)
            self.subThread.start()
        except socket.error as e:
            print("TCP Server Error(0)")
            print(e)
            self.client.close()
            self.serverSocket.close()

    def sendTCPMessage(self, message):
        try:
            self.client.send(message)
        except:
            self.client.close()

