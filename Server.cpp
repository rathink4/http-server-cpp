#include <iostream>
#include <cstring>
#include <winsock2.h>
#include "httpRequestResponse.h"


int initServerSocket();
void handleClient(SOCKET client);

int main() {
    SOCKET server = initServerSocket();
    
    if (server == 0) {
        return 1;
    }
    
    std::cout << "Waiting for connections..." << std::endl;

    // Accept connections to the server
    while(true) {
        SOCKET newConnection = accept(server, nullptr, nullptr);

        if (newConnection == INVALID_SOCKET) {
            std::cerr << "Server connection failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Server pinged.. Client connected!" << std::endl;

        handleClient(newConnection);
        closesocket(newConnection);
        std::cout << "Client servered.. Closed connection!" << std::endl;

    }

    closesocket(server);
    WSACleanup();

    return 0;
}

int initServerSocket() {
    WSADATA wsadata;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsadata);

    // Check if initialization is success
    if (wsaerr != 0) {
        std::cerr << "The Winsock dll was not found! \n";
        return 0;
    } else {
        std::cerr << "Server start.. \n";
    }

    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check if the socket creation is success
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Server socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }

    // Allow socket to be reused
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in service;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_family = AF_INET;
    service.sin_port = htons(8080);
    
    // Bind the socket to the IP addr and port
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Server bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }

    // Listen on the socket
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cerr << "Server listen failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }

    std::cout << "Server listening on port 8080...." << std::endl;

    return serverSocket;


}

void handleClient(SOCKET client) {
    // Receive data and send some data back
    const int BUFFER_SIZE = 4096;
    char receiveBuffer[BUFFER_SIZE] = {0};

    int rbytes = recv(client, receiveBuffer, BUFFER_SIZE-1, 0);
    
    if (rbytes <= 0) {
        std::cerr << "Server receive failed: " << WSAGetLastError() << std::endl;
        return;
    } 
    
    receiveBuffer[rbytes] = '\0';
    std::cout << "Received HTTP request:\n " << receiveBuffer << std::endl;
    std::cout << "-------------------------" << std::endl;


    try {
        auto request = parseHttpRequest(receiveBuffer);
        std::cout << "Parsed - Method: " << request.method << ", Path: " << request.path << std::endl;

        auto response = route(request);
        std::string responseStr = formatHttpResponse(response);

        int sbytes = send(client, responseStr.c_str(), (int)responseStr.length(), 0);
        if (sbytes == SOCKET_ERROR) {
            std::cerr << "Server send failed: " << WSAGetLastError() << std::endl;
        } else {
            std::cout << "Response sent successfully (" << sbytes << "bytes)" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error handling request: " << e.what() << std::endl;

        std::string errResponse = 
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 50\r\n"
            "\r\n"
            "<html><body><h1>Internal Server Error</h1></body></html>";
        
        send(client, errResponse.c_str(), (int)errResponse.length(), 0);
    }
    

}