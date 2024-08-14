#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BS 1024 // buffer size

string generateHttpResponse(const string& content) {
    string res = "";
    res += "HTTP/1.1 200 OK\r\n";
    res += "Content-Type: text/html\r\n";
    res += "Content-Length: " + to_string(content.length()) + "\r\n";
    res += "Connection: close\r\n";
    res += "\r\n";
    res += content;

    return res;
}

string handleRequest(const string& request) {
    istringstream requestStream(request);
    string method, path, version;
    requestStream >> method >> path >> version;

    if (method == "GET" && path == "/") {
        string content = "<html><body><h1>Welcome to the C++ HTTP Server!</h1></body></html>";
        return generateHttpResponse(content);
    }
    else {
        string content = "<html><body><h1>404 Not Found</h1></body></html>";
        return generateHttpResponse(content);
    }
}

int main() {
    WSADATA wsadata;
    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (res != 0) {
        cout << "WSAStartup failed: " << res << endl;
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        cout << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        cout << "Bind failed" << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed" << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    cout << "HTTP server listening on port " << PORT << endl;

    while (true) {
        SOCKET client = accept(server, nullptr, nullptr);

        if (client == INVALID_SOCKET) {
            cout << "Can't accept connection" << endl;
            continue;
        }

        char buffer[BS] = { 0 };
        int bytesReceived = recv(client, buffer, BS, 0);

        if (bytesReceived > 0) {
            string req(buffer, bytesReceived);
            cout << "Received request: " << req << endl;

            string res = handleRequest(req);
            int bytesSent = send(client, res.c_str(), res.length(), 0);

            if (bytesSent == SOCKET_ERROR) {
                cout << "Send failed" << endl;
            }
        }
        else if (bytesReceived == SOCKET_ERROR) {
            cout << "Receive failed" << endl;
        }

        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
