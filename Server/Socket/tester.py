#! /usr/bin/env python
# Gateway Unit Tester
# @author: Luke Hovington

import sys
import socket
import optparse
import time
import struct, crc16
import umsgpack

p = optparse.OptionParser()
p.add_option('--host', dest='lis_addr', type='str', help='listener host')
p.add_option('--port', dest='lis_port', type='int', help='listener port')
p.set_defaults(lis_port=8001, lis_addr="localhost")

(opts, args) = p.parse_args()
globals().update(opts.__dict__)

device_name = 'front-door'
mac_address = "00:23:6c:95:d1:c6"
version_num = 1
device_salt = "sx"
rfid_cards  = [12344010, 3090264] # Number on cards, with leading zero's removed

s = None

for res in socket.getaddrinfo(lis_addr, lis_port, socket.AF_UNSPEC, socket.SOCK_STREAM):
    af, socktype, proto, canonname, sa = res
    try:
        s = socket.socket(af, socktype, proto)
        s.settimeout(10)
    except socket.error, msg:
        s = None
        continue
    try:
        s.connect(sa)
    except socket.error, msg:
        s.close()
        s = None
        continue
    break
    
if s is None:
    print 'could not open socket'
    sys.exit(1)

def checkResponce(expectedReturn, returnedData):
    if returnedData[0] == expectedReturn:
        if returnedData[1] == 'E': 
            print "Error code returned.."
            print returnedData[:2], ord(returnedData[2]), ord(returnedData[3]), returnedData[4:]
            return False
            
        print "Response: %s%d"% (returnedData[0], ord(returnedData[1]) )
        if ord(returnedData[1]) == 1:
            return True
        else:
            print "Test Failed.."
    else:
        print "Bad Responce:", [returnedData]
    return False

def getToken():
    print "Requesting Token"
    print ":".join("{0:x}".format(ord(c)) for c in umsgpack.packb(['T']))
    
    s.send(umsgpack.packb(['T']))
    token = umsgpack.unpackb(s.recv(1024))
    print "Token Recived:",token
    crc = token.pop()
    if struct.unpack('<H', crc)[0] == crc16.crc16(''.join(token)):
        print "Token Valid - CRC passed"
    else:
        print "Token INVALID", crc16.crc16(token[1]), struct.unpack('<H', crc)[0]
    
    return token[1]



print "\n---- STARTING SERVER TEST ----\n"
print "Testing... Hello"

# ------------ Start Hello Message ------------

# Request a token
token = getToken()

# Send Hello Message
device_len = struct.pack('<B', len(device_name))
#print len(device_name)
mac_add = ''
for octet in mac_address.split(":"):
    mac_add += struct.pack('<B', int(octet, 16))
v_num = struct.pack('<B', version_num)
send_hello = ['H', device_name, mac_add, v_num]
send_hello.append(struct.pack('<H', crc16.crc16(''.join(send_hello) + device_salt + token)))

print send_hello
print ":".join("{0:x}".format(ord(c)) for c in umsgpack.packb(send_hello))
s.send(umsgpack.packb(send_hello))
hello = umsgpack.unpackb(s.recv(1024))
if checkResponce('H', hello):
    print "Hello, PASS"
else:
    print "Hello, FAIL"



# ------------ Send a Heartbeat ------------
print "\nTesting... HeartBeat"

send_heartBeat = ['B', mac_add]
print ''.join(send_heartBeat)
send_heartBeat.append(struct.pack('<H', crc16.crc16(''.join(send_heartBeat))))
print send_heartBeat
s.send(umsgpack.packb(send_heartBeat))

heartBeat = umsgpack.unpackb(s.recv(1024))
if checkResponce('B', heartBeat):
    print "HeartBeat, PASS"
else:
    print "HeartBeat, FAIL"



# ------------ Start Authentication ------------
print "\nTesting... Authentication"
time.sleep(5) # wait 5 seconds
old_token = token

# Request a token
token = getToken()

if old_token != token:
    print "Token has changed!!"
else:
    print "Token FALURE, expecting diffrent token"


# Send a Auth Request
device_len = struct.pack('<B', len(device_name))

for card in rfid_cards:
    print
    print "---- Checking for card ", card, "----"
    send_auth = ['@', mac_add, struct.pack('<L', card)]
    print device_salt, token
    send_auth.append(struct.pack('<H', crc16.crc16(''.join(send_auth) + device_salt + token)))
    print send_auth
    print ":".join("{0:x}".format(ord(c)) for c in umsgpack.packb(send_auth))
    s.send(umsgpack.packb(send_auth))
    auth = umsgpack.unpackb(s.recv(1024))
    
    if checkResponce('@', auth):
        print "Auth, PASS"
        crc = struct.unpack('<H',auth.pop())[0]
        if crc == crc16.crc16(''.join(auth) + device_salt + token):
            print "Auth - CRC passed"
        else:
            print "Auth CRC INVALID", crc16.crc16(''.join(auth) + device_salt + token), crc
    else:
        print "Auth, FAIL"


s.close()

print