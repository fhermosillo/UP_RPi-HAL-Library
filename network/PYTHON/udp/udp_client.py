# -*- coding: utf-8 -*-
"""
Created on Wed Oct 12 01:07:52 2022
@author: User123
"""

from socket import *

s = socket(AF_INET, SOCK_DGRAM)
serverAddressPort   = ("169.254.154.192", 20001)


s.sendto(str.encode('Hola raspy!'), serverAddressPort)
udpmsg=s.recvfrom(1024)
msg=udpmsg[0]
addr=udpmsg[1]
print('Server: ', addr, ', Message: ', msg)
s.sendto(str.encode('Como estas?'), serverAddressPort)
udpmsg=s.recvfrom(1024)
msg=udpmsg[0]
addr=udpmsg[1]
print('Server: ', addr, ', Message: ', msg)
s.close()
Footer
© 2022 GitHub, Inc.
Footer navigation
Terms
Priv
