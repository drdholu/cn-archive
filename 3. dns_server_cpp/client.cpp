#include <winsock2.h>
#include <iostream>
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void run_dns_client() {
    WSADATA wsaData;
    SOCKET client_socket;
    sockaddr_in server_addr;
    char buffer[512];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return;
    }

    // Create a UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket == INVALID_SOCKET) {
        cerr << "Failed to create socket." << endl;
        WSACleanup();
        return;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    string domain_name;
    cout << "Enter domain name: ";
    getline(cin, domain_name);

    // Send domain name to the DNS server
    sendto(client_socket, domain_name.c_str(), domain_name.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));

    // Receive response from the DNS server
    int server_addr_len = sizeof(server_addr);
    int recv_len = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&server_addr, &server_addr_len);
    if (recv_len == SOCKET_ERROR) {
        cerr << "Receive failed." << endl;
        closesocket(client_socket);
        WSACleanup();
        return;
    }

    buffer[recv_len] = '\0';
    cout << "Domain name " << domain_name << " resolves to " << buffer << endl;

    // Cleanup
    closesocket(client_socket);
    WSACleanup();
}

int main() {
    run_dns_client();
    return 0;
}
