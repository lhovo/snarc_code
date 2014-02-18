#!/usr/bin/env python
# Access Gateway Controller
# @author: Luke Hovington

from datetime import datetime, timedelta
import time, calendar, os
import crc16, struct
import socket, select, sys
from response import *
import optparse
import threading
import umsgpack

# Sanity check on startup
for descriptor in responseLookup:
    if len(descriptor) > int('FF', 16): # hex 255
        print "ERROR on descriptor", descriptor, "Description too long"
        sys.exit(1)
        
        if errorLookup[descriptor] > int('FF', 16): # hex 255
           print "ERROR on id", id, "id too long"
           sys.exit(1)

def client_parse(rfidInput, client, client_socket):
    clientReturn = None
    try:
        message = umsgpack.unpackb(rfidInput);
        print message
    except TypeError:
        message = ['']

    if message[0] in socket_options:
        clientReturn = socket_options[message[0]]['d'](message)
    else:
        clientReturn = {'response':response(rfidInput[0], 'InvalidOption')}
    
    if clientReturn == None:
        return
    
    if 'response' not in clientReturn:
        pass
    elif clientReturn['response'] is '':
        client_socket.send(umsgpack.packb(response(rfidInput[0], 'ServerError')))
    else:
        print "Returning", clientReturn['response'], "to", client['addr']
        client_socket.send(umsgpack.packb(clientReturn['response']))
        print ":".join("{0:x}".format(ord(c)) for c in umsgpack.packb(clientReturn['response']))

def clientReset(client):
    client['msgbuf'] = []
    client['remain'] = 0
    client['bytes'] = 0

def client_rxbyte(client, byte, conn):
    ''' Parse a byte received from a client. Once we receive an entire packet,
    send the packet out over serial. '''
    client['msgbuf'].append(byte)
    msgbuf = client['msgbuf']
    
    print "Recived byte 0x%02X"%ord(msgbuf[-1]), len(msgbuf), client['bytes'], client['remain']
    
    if (ord(byte) & 0xF0) == 0x90 and len(msgbuf) > 1:
        # Another Packet has started?
        # Discard other packet and start proccessing this one
        clientReset(client)
        client['msgbuf'].append(byte)
        msgbuf = client['msgbuf'];

    # Messages start with (9x):c4:1:
    # Clear out any bad packets
    if ((ord(msgbuf[0]) & 0xF0) != 0x90): # or (len(msgbuf) > 3 and msgbuf[3] not in socket_options):
        clientReset(client)
        print "clearing bytes from buffer"
    elif len(msgbuf) == 1:
        client['remain'] = (ord(msgbuf[0]) & 0x0F)
        client['bytes'] = 1

    # get packet length
    if len(msgbuf) > 2:
        if (client['bytes'] < len(msgbuf) and ord(msgbuf[-2]) == 0xC4):
           client['bytes'] += ord(msgbuf[-1])+2
           client['remain'] -= 1
           print "Input Size", len(msgbuf), (ord(msgbuf[0]) & 0x0F), client['bytes']
        if len(msgbuf) >= client['bytes']:
            if client['remain'] == 0:
                if (socket_options[msgbuf[3]]['l'] == (ord(msgbuf[0]) & 0x0F)) or socket_options[msgbuf[3]]['l'] == 0:
                    print "\nFull Packet", client['addr']
                    client_parse(''.join(msgbuf), client, conn)
                clientReset(client)
            elif len(msgbuf) != client['bytes'] and ord(msgbuf[-1]) != 0xC4:
                print "\nBad stuff....", len(msgbuf), client['bytes'], client['remain']
                clientReset(client)


# Rotate the token every 5 second
def token_rotation():
    while runThreads:
        with token_lock:
            tokens.pop()
            tokens.insert(0, gen_token())
            #print tokens
        time.sleep(5)

token_thread = threading.Thread(name='TokenRotator', target=token_rotation);
token_thread.start();

HOST = None               # Symbolic name meaning all available interfaces
s = None
PORT=8001

for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM, 0, socket.AI_PASSIVE):
    af, socktype, proto, canonname, sa = res
    try:
        s = socket.socket(af, socktype, proto)
    except socket.error, msg:
        s = None
        continue
    try:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(sa)
        s.listen(10) # Listen for 10 clients
        s.settimeout(1)
    except socket.error, msg:
        s.close()
        s = None
        continue
    break
if s is None:
    runThreads = False
    print >> sys.stderr, 'Could not open socket'
    sys.exit(1)

clients = {}
try:
    select_timeout = 0.05
    while True:
        # wait until one of:
        #   1. A new client connects on one of the accept sockets
        #   2. New data comes in from one of the clients
        #   3. A timeout
        rr, rw, err = select.select([s] + clients.keys(), [], [], select_timeout)
        
        try:
            for sock in rr:
                # are there any new client connections?
                if sock == s:
                    (csock, (addr, port)) = sock.accept()
                    csock.setblocking(0)
                    clients[csock] = {'addr':addr, 'msgbuf':[], 'remain':0, 'bytes':0}
                    print >> sys.stderr, 'New connection from %s' % (addr)
                
                # have any of the clients sent us data?
                elif sock in clients:
                    try:
                        rxbytes = sock.recv(100)
                        if rxbytes == '':
                            raise socket.error
                        for byte in rxbytes:
                            try:
                                client_rxbyte(clients[sock], byte, sock)
                            except (IndexError, umsgpack.InsufficientDataException, TypeError, struct.error) as e:
                                print "Unexpected error:", sys.exc_info()[0]
                                print e
                                clients[sock] = {'addr':clients[sock]['addr'], 'msgbuf':[], 'remain':0, 'bytes':0}
                                client_socket.send(umsgpack.packb(response(rfidInput[0], 'ServerError')))
                    except socket.error:
                        # client died, or connection lost
                        c = clients.pop(sock)
                        print >> sys.stderr,  'Client from %s disconnected' % c['addr']
                        sock.close()
            
#            s.send(str(node_data_store))
        except socket.timeout:
            pass
except KeyboardInterrupt:
    runThreads = False
    print "\nPlease wait for connections to close.."
