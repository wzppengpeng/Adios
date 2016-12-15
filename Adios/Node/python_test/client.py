#encoding:utf-8

import socket
import time
from sys import argv

ip = "10.210.61.140"
port = 9000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, port))
s.send(argv[1])
d = s.recv(1024)
print(d)
s.close()