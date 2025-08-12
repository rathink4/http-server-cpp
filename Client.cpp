#include <iostream>
#include <winsock2.h>

int initClient();

int main() {
    SOCKET client = initClient();

    if (client == 0) {
        return 1;
    }

    sockaddr_in clientService;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(8080);

    // Connect to the server 
    if (connect(client, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
        std::cerr << "Client connect failed: " << WSAGetLastError() << std::endl;
        closesocket(client);
        WSACleanup();
        return 1;
    }

    char sendBuffer[1024] = {0};
    char receiveBuffer[1024] = {0};

    int sbufferLen = 1024;
    int rbufferLen = 1024;

    // Send and receive data from the server
    std::cout << "Enter message: ";
    std::cin.getline(sendBuffer, sbufferLen);
    int messageLen = strlen(sendBuffer);

    if (send(client, sendBuffer, messageLen, 0) == SOCKET_ERROR) {
        std::cerr << "Client request failed: " << WSAGetLastError() << std::endl;
    }

    int rbytes = recv(client, receiveBuffer, rbufferLen-1, 0);
    if (rbytes == SOCKET_ERROR) {
        std::cerr << "Client receive failed: " << WSAGetLastError() << std::endl;
    } else if (rbytes > 0) {
        receiveBuffer[rbytes] = '\0';
        std::cout << "Client received response: " << receiveBuffer << std::endl;
    }

    closesocket(client);
    WSACleanup();

    return 0;
}

int initClient() {
    WSADATA wsadata;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsadata);

    // Check if initialization is success
    if (wsaerr != 0) {
        std::cerr << "The Winsock dll was not found! \n";
        return 0;
    } else {
        std::cerr << "Client start.. \n";
    }

    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check if the socket creation is success
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Client socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    
    return clientSocket;

}