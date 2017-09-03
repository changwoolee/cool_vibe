import socket, threading

class RpiServerThread(threading.Thread):
    def __init__(self, connection,clientAddress):
        threading.Thread.__init__(self)

        self.connection = connection
        self.cliendAddress = clientAddress


    def run(self):
        try:
            while True:
                data = self.connection.recv(1024).decode()

                if not data:
                    print("Connection Lost")
                    break

                # Handle data from Unity
                print(data)
                
        except:
            exit(0)
