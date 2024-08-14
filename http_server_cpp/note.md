# Overview of code

```cpp
#include <iostream>
#include <sstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
```
- **#include <iostream>:** This includes the standard input-output stream library in C++. It's used for handling input and output operations.
- **#include <sstream>:** This includes the string stream library, which is used for performing input and output operations on strings.
- **#include <string>:** This includes the string library, which provides support for manipulating strings.
- **#include <winsock2.h>:** This includes the Winsock2 library, which provides the necessary functions and definitions for network programming on Windows.
- **#include <ws2tcpip.h>:** This includes the Winsock2 TCP/IP library, which extends Winsock2 with support for newer networking features.

```cpp
#pragma comment(lib, "Ws2_32.lib")
```
- **#pragma comment(lib, "Ws2_32.lib"):** This line tells the linker to link the Ws2_32.lib library, which contains the Winsock functions.

```cpp
const int PORT = 8080;
const int BUFFER_SIZE = 1024;
```
- **const int PORT = 8080:** This defines a constant integer `PORT` with a value of 8080. This is the port number on which the server will listen for incoming connections.
- **const int BUFFER_SIZE = 1024:** This defines a constant integer `BUFFER_SIZE` with a value of 1024. This will be the size of the buffer used to receive data.

```cpp
std::string generateHttpResponse(const std::string& content) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << content;
    return response.str();
}
```
- **std::string generateHttpResponse(const std::string& content):** This function generates an HTTP response given the content to be included in the response.
  - **std::ostringstream response:** This creates a string stream object named `response` to build the response string.
  - **response << "HTTP/1.1 200 OK\r\n"...;** This appends various parts of the HTTP response to the string stream, including:
    - The HTTP status line indicating a successful response (200 OK).
    - Headers specifying the content type (text/html), content length, and connection status.
    - The actual content of the response.
  - **return response.str();** This converts the string stream to a standard string and returns it.

```cpp
std::string handleRequest(const std::string& request) {
    std::istringstream requestStream(request);
    std::string method, path, version;
    requestStream >> method >> path >> version;

    if (method == "GET" && path == "/") {
        std::string content = "<html><body><h1>Welcome to the C++ HTTP Server!</h1></body></html>";
        return generateHttpResponse(content);
    } else {
        std::string content = "<html><body><h1>404 Not Found</h1></body></html>";
        return generateHttpResponse(content);
    }
}
```
- **std::string handleRequest(const std::string& request):** This function processes the HTTP request and generates an appropriate response.
  - **std::istringstream requestStream(request):** This creates a string stream object to parse the request string.
  - **requestStream >> method >> path >> version:** This extracts the HTTP method, path, and version from the request.
  - **if (method == "GET" && path == "/") {...} else {...}:** This checks if the request is a GET request for the root path ("/"). If it is, it generates a 200 OK response with a welcome message. Otherwise, it generates a 404 Not Found response.

```cpp
int main() {
    WSADATA wsaData;
    int iResult;
```
- **int main():** This is the main function where the program execution starts.
- **WSADATA wsaData;** This declares a variable to hold information about the Windows Sockets implementation.
- **int iResult;** This declares a variable to store the result of function calls.

```cpp
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }
```
- **iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);** This initializes the Winsock library. `MAKEWORD(2, 2)` specifies that we want to use Winsock version 2.2.
- **if (iResult != 0) {...}** This checks if `WSAStartup` succeeded. If it failed, it prints an error message and exits the program.

```cpp
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
```
- **SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);** This creates a new socket. `AF_INET` specifies the address family (IPv4), `SOCK_STREAM` specifies the socket type (stream socket, for TCP), and `IPPROTO_TCP` specifies the protocol (TCP).
- **if (server_fd == INVALID_SOCKET) {...}** This checks if the socket creation succeeded. If it failed, it prints an error message, cleans up Winsock resources, and exits the program.

```cpp
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
```
- **sockaddr_in address;** This declares a variable to hold the server address information.
- **address.sin_family = AF_INET;** This sets the address family to IPv4.
- **address.sin_addr.s_addr = INADDR_ANY;** This sets the IP address to accept connections from any IP address.
- **address.sin_port = htons(PORT);** This sets the port number. `htons` converts the port number from host byte order to network byte order.

```cpp
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
```
- **if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {...}** This binds the socket to the specified address and port. If binding fails, it prints an error message, closes the socket, cleans up Winsock resources, and exits the program.

```cpp
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
```
- **if (listen(server_fd, 3) == SOCKET_ERROR) {...}** This puts the socket in listening mode to accept incoming connections. The second argument specifies the maximum length of the queue of pending connections (3 in this case). If listening fails, it prints an error message, closes the socket, cleans up Winsock resources, and exits the program.

```cpp
    std::cout << "HTTP server listening on port " << PORT << std::endl;
```
- **std::cout << "HTTP server listening on port " << PORT << std::endl;** This prints a message indicating that the server is listening for connections on the specified port.

```cpp
    while (true) {
        SOCKET new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }
```
- **while (true) {...}** This is an infinite loop that continuously waits for and handles incoming connections.
- **SOCKET new_socket = accept(server_fd, nullptr, nullptr);** This accepts a new connection from a client. If the accept fails, it prints an error message, closes the server socket, cleans up Winsock resources, and exits the program.

```cpp
        char buffer[BUFFER_SIZE] = {0};
        int bytesReceived = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
            closesocket(new_socket);
            continue;
        }
```
- **char buffer[BUFFER_SIZE] = {0};** This creates a buffer to store the received data, initializing all elements to 0.
- **int bytesReceived = recv(new_socket, buffer, BUFFER_SIZE, 0);** This receives data from the client and stores it in the buffer. The `recv` function returns the number of bytes received. If receiving fails, it prints an error message, closes the client socket, and continues

 to the next iteration of the loop.

```cpp
        std::string request(buffer, bytesReceived);
        std::string response = handleRequest(request);
```
- **std::string request(buffer, bytesReceived);** This creates a string from the received data.
- **std::string response = handleRequest(request);** This handles the request and generates an appropriate response using the `handleRequest` function.

```cpp
        send(new_socket, response.c_str(), response.length(), 0);
        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
```
- **send(new_socket, response.c_str(), response.length(), 0);** This sends the generated response back to the client.
- **closesocket(new_socket);** This closes the client socket.
- **closesocket(server_fd);** This closes the server socket.
- **WSACleanup();** This cleans up Winsock resources.
- **return 0;** This returns 0, indicating that the program executed successfully.

This should give you a comprehensive understanding of each part of the code. Feel free to ask if you have any specific questions!