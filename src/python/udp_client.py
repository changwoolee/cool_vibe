from socket import *
import threading
import sys

class UdpClient(threading.Thread):
	def __init__(self,HOST,PORT,listener):
		threading.Thread.__init__(self)
		self.HOST = HOST
		self.PORT = PORT
		self.BUFSIZE = 1024
		self.ADDR = (HOST,PORT)
		self.listener = listener
		self.udpClient = socket(AF_INET,SOCK_DGRAM)
		self.udpClient.bind(('',self.PORT))
	def run(self):
		while True:
			data, addr = self.udpClient.recvfrom(self.BUFSIZE)
			self.listener(data)
	def send(self,data):
		self.udpClient.sendto(data,self.ADDR)
	def __del__(self):
		self.udpClient.close()
if __name__=="__main__":
	def recv_listener(data):
		print(data)
		
	udpClient = UdpClient("192.168.0.14",7000, recv_listener)
	udpClient.daemon = True	
	udpClient.start()
	
	while True:
		data = raw_input()
		udpClient.send(data)
	
