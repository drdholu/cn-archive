#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <winsock2.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

vector<SOCKET> clients;
mutex clients_mutex;

void broadcast_message(const string& message, SOCKET sender_socket) {
    lock_guard<mutex> lock(clients_mutex);
    for (SOCKET client_socket : clients) {
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    string welcome_message = "Welcome to the chat room!\n";
    send(client_socket, welcome_message.c_str(), welcome_message.size(), 0);

    while (true) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';

        // Get current time
        time_t now = time(0);       
        char dt[26];                
        errno_t err = ctime_s(dt, sizeof(dt), &now);

        if (err == 0) {            
            string time_str(dt); 
            time_str.pop_back();   
            string message = "[" + time_str + "] " + string(buffer);

            broadcast_message(message, client_socket); // Send the message to all clients
        }
    }

    // Remove client and close socket
    {
        lock_guard<mutex> lock(clients_mutex);
        clients.erase(remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    closesocket(client_socket);

    // Broadcast that the client has left
    string leave_message = "A user has left the chat.\n";
    broadcast_message(leave_message, INVALID_SOCKET);
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cerr << "Failed to initialize Winsock. Error Code: " << WSAGetLastError() << endl;
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        cerr << "Could not create socket. Error Code: " << WSAGetLastError() << endl;
        return 1;
    }

    // Prepare sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed. Error Code: " << WSAGetLastError() << endl;
        return 1;
    }

    // Listen for incoming connections
    listen(server_socket, 10);
    cout << "Server started on port " << PORT << endl;

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            cerr << "Accept failed. Error Code: " << WSAGetLastError() << endl;
            continue;
        }

        // Add client to the list
        {
            lock_guard<mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }

        // Create a new thread for this client
        thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    // Clean up
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
