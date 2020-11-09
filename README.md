
# tcp-sockets-in-c

#### Basic socket programming exercise in c.
##### Problem Statement
> _*String Reversal Server using TCP Sockets*_ 
The problem is to implement a client - server user-level application using sockets API in C. The Server application has to support at least five clients simultaneously. Server accepts strings from clients (even multiple strings from each client) and replies with reversed strings. For example, when client sends “UOHYD”, Server replies with “DYHOU”. Both server and client(s) have to output both sending & receiving strings on the terminal. The server and client processes should be run on different machines. During evaluation, you will be asked to setup up to 5 client processes on different machines.

##### Installation / Compilation
	>> clone https://github.com/Subhash3/tcp-sockets-in-c
	>> cd tcp-sockets-in-c
	>> make
##### Execution
- Open Two terminals, one for the server and the other for the client.
- Run Server
	```./server```
- Run Client
	```./client <SERVER_IP> <SERVER_PORT>```
- SERVER_PORT is hardcoded to 12345 in ```server.c```, feel free to change it to whatever you'd like.


