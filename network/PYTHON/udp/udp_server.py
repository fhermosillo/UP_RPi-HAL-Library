from socket import *

s = socket(AF_INET, SOCK_DGRAM)
s.bind(('169.254.154.192', 20001))

# UDP es un protocolo sin control de acceso
# Puede recivir de cualquier dispositivo
# sin ser aceptado previamente
udpmsg=s.recvfrom(1024)
msg=udpmsg[0]
addr=udpmsg[1]
print('Client: ', addr, ', Message: ', msg)
s.sendto(str.encode('Hola windows'),addr)
udpmsg=s.recvfrom(1024)
msg=udpmsg[0]
addr=udpmsg[1]
print('Client: ', addr, ', Message: ', msg)
s.sendto(str.encode('Bien gracias!'), addr)
s.close()
Footer
© 2022 GitHub, Inc.
Footer navigation
Ter
