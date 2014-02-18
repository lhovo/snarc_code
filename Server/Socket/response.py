#!/usr/bin/env python
# Access Gateway Controller
# @author: Luke Hovington

import random, string, thread
import crc16, struct
from dob import *
import unittest

debug = False

# The first 19 are responces not errors
responseLookup = {
    'Unknown'      :0,
    'Ok'           :1,
    'Fail'         :2,
    
    'SwitchOn'     :10,
    'Resume'       :11,
    'SwitchOff'    :12,
    'MomentaryOn'  :13,
    
    'ChecksumFail' :20,
    'InvalidOption':21,  
    'UnknownError' :22,
    'ServerError'  :23,
}

socket_options = {}
def init_socket_options():
    global socket_options
    # Expected packet length minus the device name
    socket_options = {
        'T':{'l':1, 'd':sendToken},
        'H':{'l':5, 'd':hello},
        'B':{'l':3, 'd':heartBeat},
        '@':{'l':4, 'd':authenticate},
        'A':{'l':4, 'd':acknowledge},
    }

def debugPrint(*x):
    if debug:
        print x

def gen_token():
    foo = random.SystemRandom()
    length = 2
    chars = string.printable
    return ''.join(foo.choice(chars) for _ in xrange(length))

# Start with two random tokens, the first one will be passed back when asked.
# The second will be checked if the first fails (might be due to recently changed) 
tokens = [gen_token(), gen_token()]
token_lock = thread.allocate_lock()
runThreads = True

# Helper to construct return packets
def response(code, descriptor):
    if descriptor in responseLookup:
        if responseLookup[descriptor] > 19:
            return [code, 'E', struct.pack('<B',responseLookup[descriptor]), descriptor]
        else:
            return [code, struct.pack('<B',responseLookup[descriptor])]
    return [code, 'E', str(responseLookup['ServerError']), 'ServerError']


# Send Token Responce 'T'
def sendToken(rfidInput):
    global token_lock, tokens
    t = ['T']
    with token_lock:
        t.append(tokens[0])
    return {'response':['T',t[1],struct.pack('<H', crc16.crc16(''.join(t)))]}

# Register Hello Packet, Update Dob and return
def hello(rfidInput):
#    dnl = struct.unpack('<B',rfidInput[1])[0] # device name length
#    packetcrc = struct.unpack('<H',''.join(rfidInput[-2:]))[0]

    device_name = rfidInput[1]
    version_number = ord(rfidInput[3])

    mac_addr = []
    for octet in rfidInput[2]:
        mac_addr.append("%02x"%ord(octet))
    mac_addr = ":".join(mac_addr)
    
    device_salt = dob_get_salt(mac_addr)
    
    debugPrint( "device name", device_name )
    debugPrint( "mac addr" , mac_addr )
    debugPrint( "Version Number", version_number )

    packetcrc = struct.unpack('<H',rfidInput.pop())[0]
    calculate_crc = []
    with token_lock:
        calculate_crc.append(crc16.crc16(''.join(rfidInput)+device_salt+tokens[0]))
        calculate_crc.append(crc16.crc16(''.join(rfidInput)+device_salt+tokens[1]))

    if packetcrc in calculate_crc:
        if dob_hello_check(device_name, mac_addr, version_number):
            return {'response':response(rfidInput[0], 'Ok')}
        else:
            return {'response':response(rfidInput[0], 'Unknown')}
    else:
        return {'response':response(rfidInput[0], 'ChecksumFail')}

# Register Heart Beat with the database
def heartBeat(rfidInput):
    mac_addr = []
    for octet in rfidInput[2]:
        mac_addr.append("%02x"%ord(octet))
    mac_addr = ":".join(mac_addr)
    
    if dob_heartBeat_register(mac_addr):
        return {'response':response(rfidInput[0], 'Ok')}
    else:
        return {'response':response(rfidInput[0], 'Fail')}
    
# Check user is allowed in
def authenticate(rfidInput):
    packetcrc = struct.unpack('<H',rfidInput.pop())[0]
    scanned_card = struct.unpack('<L', ''.join(rfidInput[2]))[0]
    
    mac_addr = []
    for octet in rfidInput[2]:
        mac_addr.append("%02x"%ord(octet))
    mac_addr = ":".join(mac_addr)
    device_salt = dob_get_salt(mac_addr)
    
    calculate_crc = []
    tokens_list = []
    with token_lock:
        calculate_crc.append(crc16.crc16(''.join(rfidInput)+device_salt+tokens[0]))
        calculate_crc.append(crc16.crc16(''.join(rfidInput)+device_salt+tokens[1]))
        # Do a copy not DON'T point to the data
        tokens_list.append(tokens[0])
        tokens_list.append(tokens[1])

    if packetcrc in calculate_crc:
        if packetcrc == calculate_crc[0]:
            tokens_list.pop(1)
        else:
            tokens_list.pop(0)
        
        if dob_check_user(mac_addr, scanned_card):
            resp = response(rfidInput[0], 'Ok')
        else:
            resp = response(rfidInput[0], 'Fail')
    else:
        return {'response':response(rfidInput[0], 'ChecksumFail')}
    resp.append(struct.pack('<H', crc16.crc16(''.join(resp)+device_salt+tokens_list[0])))
    return {'response': resp}

def acknowledge():
    return {}


class ReponseTestCase(unittest.TestCase):
    device_name = 'front-door'
    mac_address = "00:23:6c:95:d1:c6"
    version_num = 1
    device_salt = "sx"
    allowed_cards_list = [12344010]
    deny_cards_list    = [3090264]
    
    def checkResponce(self, expectedReturn, returnedData):
        if returnedData[0] == expectedReturn:
            if returnedData[1] == 'E': 
                debugPrint( "Error code returned.." )
                debugPrint( returnedData )
                return False
                
            debugPrint( "Response: %s%d"% (returnedData[0], ord(returnedData[1])) )
            if ord(returnedData[1]) == 1:
                return True
            else:
                debugPrint( "Test Failed.." )
        else:
            debugPrint( "Bad Responce:", returnedData )
        return False
    
    def test_sendToken_token(self):
        t = sendToken(['T'])['response']
        with token_lock:
            self.assertEqual(t[1], tokens[0])
            
    def test_sendToken_crc(self):
        t = sendToken(['T'])['response']
        crc = t.pop()
        self.assertEqual(crc16.crc16(''.join(t)), struct.unpack('<H', crc)[0])

    def test_sendHello(self):
        device_len = len(self.device_name)
        mac_add = ''
        for octet in self.mac_address.split(":"):
            mac_add += struct.pack('<B', int(octet, 16))
        send_hello = ['H', self.device_name, mac_add, chr(self.version_num)]
        send_hello.append(struct.pack('<H', crc16.crc16(''.join(send_hello) + self.device_salt + tokens[0])))
        
        h = hello(send_hello)['response']
        self.failUnless(self.checkResponce('H',h))

    def test_heartBeat(self):
        mac_add = ''
        for octet in self.mac_address.split(":"):
            mac_add += str(int(octet, 16))
        
        send_heartBeat = ['B',  mac_add]
        send_heartBeat.append(struct.pack('<H', crc16.crc16(''.join(send_heartBeat))))
        
        b = heartBeat(send_heartBeat)['response']
        self.failUnless(self.checkResponce('B',b))
    
    def test_auth_CRC(self):
        mac_add = ''
        for octet in self.mac_address.split(":"):
            mac_add += struct.pack('<B', int(octet, 16))
        
        send_auth = ['@', mac_add, struct.pack('<L', self.allowed_cards_list[0])]
        send_auth.append(struct.pack('<H', crc16.crc16(''.join(send_auth) + self.device_salt + tokens[0])))
        
        auth = authenticate(send_auth)['response']
        self.assertEqual(struct.unpack('<H', auth[-1])[0], crc16.crc16(''.join(auth[:-1]) + self.device_salt + tokens[0]))

    def test_auth_allowed(self):
        mac_add = ''
        for octet in self.mac_address.split(":"):
            mac_add += str(int(octet, 16))
        
        for card in self.allowed_cards_list:
            send_auth = ['@', mac_add, struct.pack('<L', card)]
            send_auth.append(struct.pack('<H', crc16.crc16(''.join(send_auth) + self.device_salt + tokens[0])))
            
            auth = authenticate(send_auth)['response']
            self.failUnless(self.checkResponce('@',auth))

    def test_auth_deny(self):
        mac_add = ''
        for octet in self.mac_address.split(":"):
            mac_add += str(int(octet, 16))
		
        for card in self.deny_cards_list:
            send_auth = ['@', mac_add, struct.pack('<L', card)]
            send_auth.append(struct.pack('<H', crc16.crc16(''.join(send_auth) + self.device_salt + tokens[0])))
            
            auth = authenticate(send_auth)['response']
            self.failIf(self.checkResponce('@',auth))

init_socket_options()

if __name__ == '__main__':
    unittest.main()