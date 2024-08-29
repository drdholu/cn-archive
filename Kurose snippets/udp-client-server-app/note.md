## Notes

- Server must attach a destination address to the packet before dropping it into the socket

## Steps of execution
- To test the pair of programs, you run UDPClient.py on one host and UDPServer.
py on another host. Be sure to include the proper hostname or IP address of the server 
in UDPClient.py. 
- Next, you execute UDPServer.py, the compiled server program, in 
the server host. This creates a process in the server that idles until it is contacted by 
some client. 
- Then you execute UDPClient.py, the compiled client program, in the 
client. This creates a process in the client. 
- Finally, to use the application at the client, 
you type a sentence followed by a carriage return.