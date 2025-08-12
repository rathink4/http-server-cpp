#include <iostream>
#include <cstring>
#include <winsock2.h>


int initServerSocket();

int main() {
    SOCKET server = initServerSocket();
    
    if (server == 0) {
        return 1;
    }

    // Accept connections to the server
    SOCKET newConnection;
    newConnection = accept(server, nullptr, nullptr);

    if (newConnection == INVALID_SOCKET) {
        std::cerr << "Server connection failed: " << WSAGetLastError() << std::endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    // Receive data and send some data back
    char receiveBuffer[1024] = {0};
    char *sendBuffer = "Hello from the server!";

    int rbufferSize = 1024;
    int rbytes = recv(newConnection, receiveBuffer, rbufferSize-1, 0);
    if (rbytes < 0) {
        std::cerr << "Server receive failed: " << WSAGetLastError() << std::endl;
    } else {
        receiveBuffer[rbytes] = '\0';
        std::cout << "Server received: " << receiveBuffer << std::endl;
    }
    
    
    if (send(newConnection, sendBuffer, (int)strlen(sendBuffer), 0) == SOCKET_ERROR) {
        std::cerr << "Server send failed: " << WSAGetLastError() << std::endl;
    }

    closesocket(newConnection);
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