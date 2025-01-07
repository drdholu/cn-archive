#include <iostream>
#include <string>
#include <ws2tcpip.h> 
#include <winsock2.h> 
using namespace std;

#pragma comment(lib, "ws2_32.lib") 

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed: " << WSAGetLastError() << endl;
        return 1;
    }

    // Get domain name input from the user
    string domain;
    cout << "Enter a domain name: ";
    getline(cin, domain);

    // DNS resolution
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC; // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result = nullptr;
    if (getaddrinfo(domain.c_str(), nullptr, &hints, &result) != 0) {
        cerr << "Error: Hostname could not be resolved." << endl;
        WSACleanup();
        return 1;
    }

    // Print the resolved IP addresses
    cout << "IP addresses for " << domain << ":" << endl;
    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        char ipStringBuffer[INET6_ADDRSTRLEN] = { 0 };

        if (ptr->ai_family == AF_INET) { // IPv4
            sockaddr_in* sockaddr_ipv4 = (sockaddr_in*)ptr->ai_addr;
            inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ipStringBuffer, sizeof(ipStringBuffer));
        }
        else if (ptr->ai_family == AF_INET6) { // IPv6
            sockaddr_in6* sockaddr_ipv6 = (sockaddr_in6*)ptr->ai_addr;
            inet_ntop(AF_INET6, &(sockaddr_ipv6->sin6_addr), ipStringBuffer, sizeof(ipStringBuffer));
        }

        cout << ipStringBuffer << endl;
    }

    // Cleanup
    freeaddrinfo(result);
    WSACleanup();

    return 0;
}
