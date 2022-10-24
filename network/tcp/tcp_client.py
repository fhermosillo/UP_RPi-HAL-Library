# -*- coding: utf-8 -*-
"""
Created on Wed Oct 12 00:50:03 2022

@author: User123
"""

from socket import *

s = socket(AF_INET, SOCK_STREAM)
s.connect(('169.254.154.192', 8888))

s.send(str.encode('Hola raspy!'))
data = s.recv(1024)
print('Server: ', data)
s.send(str.encode("Como estas?"))
data = s.recv(1024)
print('Server: ', data)
s.close()