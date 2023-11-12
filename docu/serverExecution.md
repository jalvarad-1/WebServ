# Server Execution

The "MultiServer" class is responsible for configuring and initializing multiple HTTP servers.

Receives a vector of type [serverConfig]() and for each one perform the following operations.

1. Obtain the server Port and create a [HTTPServer]() object. Later we will talk about this class, for now we will stick with what it receives:
    - 'AF_INET': The address family, in this case, it indicates that IPv4 is being used.
    - 'SOCK_STREAM': The socket type, in this case, a stream socket, which means a connection oriented connection.
    - '0': 
    - 'port': This is the port on which the server will listen for connections.
    - 'INADDR_ANY': This means that the server will accept connections from any available IP address on the system.
    - '10': The max number of connections available to a server.
    - '*it': An object the type 'ServerConfig' so that the 'HTTPServer' can access the server-specific configuration.

2. Then, we add the server to a vector.
3. Configure a pollfd structure to monitor read (POLLIN) events on a socket associated with a specific server.

### Why we use POLLIN?

POLLIN is one of the constants used in the 'poll' system call in Unix-like systems. On a server, the POLLIN signal indicates that data is available for reading in the associated file descriptor, which generally means that a request has been sent to the server.