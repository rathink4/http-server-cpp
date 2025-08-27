#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <winsock2.h>

namespace HttpServer {

    class Router;

    class Server {
        public:
            Server(const std::string& serverAddress = "127.0.0.1", int serverPort = 8080);
            ~Server();

            bool initialize();
            bool start();
            bool stop();
            
            void setAddress(const std::string& serverAddress) {address = serverAddress;}
            void setPort(int serverPort) {port = serverPort;}

            void run();



        private:

            // Server methods used to initialize and serve clients
            bool initializeWinSock();
            bool createSocket();
            bool bindSocket();
            bool listenSocket();
            bool cleanUpWinSock();


            void handleClient(SOCKET clientSocket);
            std::string receiveRequest(SOCKET clientSocket);
            bool sendResponse(SOCKET clientSocket, const std::string& generatedResponse);

            // Server attributes
            std::string address;
            int port;
            SOCKET serverSocket;

            static const int BUFFER_SIZE = 4096;
            static const int CLIENT_BACKLOG = 10;

    };

}


#endif