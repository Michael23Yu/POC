# POC
POC for CVE-2019-8956


In the Linux Kernel before versions 4.20.8 and 4.19.21 a use-after-free error in the "sctp_sendmsg()" function (net/sctp/socket.c) 
when handling SCTP_SENDALL flag can be exploited to corrupt memory.


Usage:

python3 server.py & python3 server1.py

gcc -o poc poc.c -lsctp

./poc
