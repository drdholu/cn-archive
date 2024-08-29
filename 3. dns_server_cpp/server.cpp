#include <winsock2.h>
#include <iostream>
#include <unordered_map>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

unordered_map<string, string> dns_table = {
    {"example.com", "93.184.216.34"},
    {"google.com", "142.250.190.78"},
    {"localhost", "127.0.0.1"}
};

void start_dns_server() {
    WSADATA wsaData;
    SOCKET server_socket;
    sockaddr_in server_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return;
    }

    // Create a UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == INVALID_SOCKET) {
        cerr << "Failed to create socket." << endl;
        WSACleanup();
        return;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    // Bind the socket
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed." << endl;
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    cout << "DNS Server is listening on port 53..." << endl;

    while (true) {
        char buffer[512];
        sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        // Receive data from the client
        int recv_len = recvfrom(server_socket, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_len);
        if (recv_len == SOCKET_ERROR) {
            cerr << "Receive failed." << endl;
            continue;
        }

        buffer[recv_len] = '\0';
        string domain_name(buffer);
        cout << "Received query for " << domain_name << " from " << inet_ntoa(client_addr.sin_addr) << endl;

        string ip_address = dns_table.count(domain_name) ? dns_table[domain_name] : "0.0.0.0";
        cout << "Resolved " << domain_name << " to " << ip_address << endl;

        // Send response to the client
        sendto(server_socket, ip_address.c_str(), ip_address.length(), 0, (sockaddr*)&client_addr, client_addr_len);
    }

    // Cleanup
    closesocket(server_socket);
    WSACleanup();
}

int main() {
    start_dns_server();
    return 0;
}
