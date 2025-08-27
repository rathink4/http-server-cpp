#include "server/Server.h"
#include "server/Router.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "http/HttpParser.h"
#include "http/HttpFormatter.h"
#include <iostream>
#include <cstring>

namespace HttpServer {

    Server::Server(const std::string& serverAddress, int serverPort) : address(serverAddress), port(serverPort), serverSocket(INVALID_SOCKET) {

    }

    Server::~Server() {
        stop();
    }

    bool Server::initialize() {
        if (!initializeWinSock()) {
            return false;
        }

        if (!createSocket()) {
            cleanUpWinSock();
            return false;
        }

        if (!bindSocket()) {
            closesocket(serverSocket);
            cleanUpWinSock();
            return false;
        }

        if (!listenSocket()) {
            closesocket(serverSocket);
            cleanUpWinSock();
            return false;
        }

        return true;
    }


    bool Server::start() {
        if (!initialize()) {
            return false;
        }

        std::cout << "Server listening on " << address << ":" << port << "..." << std::endl;
        return true;

    }

    bool Server::stop() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }

        return cleanUpWinSock();
    }

    void Server::run() {
        if (!start()) {
            std::cerr << "Failed to start server" << std::endl;
            return;
        }

        std::cout << "Waiting for connections..." << std::endl;

        Router router = createDefaultRouter();

        while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Server connection failed: " << WSAGetLastError() << std::endl;
                continue;
            }

            std::cout << "Server pinged.. Client connected!" << std::endl;

            handleClient(clientSocket);
            closesocket(clientSocket);

            std::cout << "Client served.. Closed connection!" << std::endl;

        }
    }

    bool Server::initializeWinSock() {
        WSADATA wsadata;
        WORD wVersionRequested = MAKEWORD(2, 2);
        int wsaerr = WSAStartup(wVersionRequested, &wsadata);

        if (wsaerr != 0) {
            std::cerr << "The Winsock dll was not found!" << std::endl;
            return false;
        }

        std::cout << "Server start.. " << std::endl;
        return true;
    }

    bool Server::createSocket() {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Server socket creation failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        return true;

    }

    bool Server::bindSocket() {
        sockaddr_in service;
        service.sin_addr.s_addr = inet_addr(address.c_str());
        service.sin_family = AF_INET;
        service.sin_port = htons(static_cast<u_short>(port));

        if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
            std::cerr << "Server bind failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        return true;

    }

    bool Server::listenSocket() {
        if (listen(serverSocket, CLIENT_BACKLOG) == SOCKET_ERROR) {
            std::cerr << "Server listen failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        return true;
    }

    bool Server::cleanUpWinSock() {
        WSACleanup();
        return true;
    }

    void Server::handleClient(SOCKET clientSocket) {
        try {

            // Receive HTTP request
            std::string requestStr = receiveRequest(clientSocket);
            
            if (requestStr.empty()) {
                std::cerr << "Received empty request." << std::endl;
                return;
            }


            std::cout << "\n\nReceived HTTP request:\n" << requestStr << std::endl;
            std::cout << "------------------------" << std::endl;

            // Parse the request
            HttpRequest request = HttpParser::parseRequest(requestStr);
            std::cout << "Parsed - Method: " << request.getMethod()
                    << ", Path: " << request.getPath() << std::endl;

            // Route the request
            Router router = createDefaultRouter();
            HttpResponse response = router.route(request);

            // Format and send response
            std::string responseStr = HttpFormatter::formatHttpResponse(response);
            if (sendResponse(clientSocket, responseStr)) {
                std::cout << "Response sent successfully ("
                        << responseStr.length() << " bytes)" << std::endl;
            }

            
        } catch (const std::exception& e) {
            std::cerr << "Error handling request: " << e.what() << std::endl;

            // Send error response
            std::string errRes = 
                "HTTP/1.1 500 Internal Server Error\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 50\r\n"
                "\r\n"
                "<html><body><h1>Internal Server Error</h1></body></html>";
            
            send(clientSocket, errRes.c_str(), static_cast<int>(errRes.length()), 0);
        }
    }

    std::string Server::receiveRequest(SOCKET clientSocket) {
        std::string requestData;
        char buffer[4096]; // Use a reasonably sized buffer
        int bytesRead = 0;
        
        // Loop to receive data until the full request is received
        do {
            memset(buffer, 0, sizeof(buffer));
            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesRead > 0) {
                requestData.append(buffer, bytesRead);
            } else if (bytesRead == 0) {
                // Connection closed
                break;
            } else {
                // An error occurred
                std::cerr << "Server receive failed: " << WSAGetLastError() << std::endl;
                return "";
            }
            
            // This is a simple check for the end of headers.
            // A more robust solution would check for Content-Length to read the body
            if (requestData.find("\r\n\r\n") != std::string::npos) {
                // Found the end of headers.
                break;
            }
        } while (bytesRead > 0);

        return requestData;

    }

    bool Server::sendResponse(SOCKET clientSocket, const std::string& generatedResponse) {
        int sbytes = send(clientSocket, generatedResponse.c_str(), static_cast<int>(generatedResponse.length()), 0);

        if (sbytes == SOCKET_ERROR) {
            std::cerr << "Server send failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        return true;
    }


}