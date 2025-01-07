#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h> 
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

void receive_messages(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        cout << buffer << endl;
    }
}

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr; 

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cerr << "Failed to initialize Winsock. Error Code: " << WSAGetLastError() << endl;
        return 1;
    }

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        cerr << "Could not create socket. Error Code: " << WSAGetLastError() << endl;
        WSACleanup();  
        return 1;
    }

    const char* server_ip = "127.0.0.1";

    // Prepare sockaddr_in structure
    server_addr.sin_family = AF_INET; 
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported" << endl;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // Set the server port number in network byte order using htons
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Connect failed. Error Code: " << WSAGetLastError() << endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    thread receive_thread(receive_messages, client_socket);
    receive_thread.detach();

    // Input loop
    string message;
    while (true) {
        getline(cin, message);
        if (message == "#EXIT") {
            break;
        }
        send(client_socket, message.c_str(), message.size(), 0);
    }

    // Clean up
    closesocket(client_socket);
    WSACleanup();

    return 0;
}