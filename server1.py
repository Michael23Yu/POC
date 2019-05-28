#!/usr/bin/env python3
# coding: utf-8

# Created by Álvaro Justen <https://github.com/turicas>

import socket
import time

socket.IPPROTO_SCTP = 132
while True:
    try:
        server = socket.socket(socket.AF_INET,socket.SOCK_SEQPACKET,socket.IPPROTO_SCTP)
        server.bind(('127.0.0.1', 1235))
        server.listen(5)
        print('Waiting for messages...')

    
        buffer = b''
        flags = None
        while flags != socket.MSG_EOR:
            # If socket.MSG_EOR is not on flags it means the message is greater
            # than the return of this call.
            # Note: here we're appending data to `buffer` without checking if
            # `sender` is the same from already buffered data (don't know if
            # this check is needed since SCTP guarantees order).
            data, ancillary_data, flags, sender = server.recvmsg(1024)
            buffer += data
            print('Received from {}: {}'.format(sender, buffer))
        #time.sleep(10)
        server.close()
       # reply = b'server reply to ' + buffer
       # server.sendmsg([reply], [], socket.MSG_EOR, sender)
       # print('Sent to {}: {}'.format(sender, reply))
    except KeyboardInterrupt:
        exit()
    except:
        continue

