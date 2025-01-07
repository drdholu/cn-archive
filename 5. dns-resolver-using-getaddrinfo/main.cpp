#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <hostname>" << std::endl;
        return 1;
    }

    std::string hostname = argv[1];

#ifdef _WIN32
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0) {
        std::cerr << "WSAStartup failed: " << wsResult << std::endl;
        return 1;
    }
#endif

    struct addrinfo hints, *result, *p;
    char ipStr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    int res = getaddrinfo(hostname.c_str(), nullptr, &hints, &result);
    if (res != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(res) << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::cout << "IP addresses for " << hostname << ":" << std::endl;

    // Loop through the result linked list and convert each address to a human-readable string
    for (p = result; p != nullptr; p = p->ai_next) {
        void* addr;
        std::string ipVersion;

        // Check if the address is IPv4 or IPv6
        if (p->ai_family == AF_INET) {  // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipVersion = "IPv4";
        } else {  // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipVersion = "IPv6";
        }

        // Convert the IP address to a string
        inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr));
        std::cout << ipVersion << ": " << ipStr << std::endl;
    }

    // Cleanup
    freeaddrinfo(result);
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
