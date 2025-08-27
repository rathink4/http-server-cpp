#include "server/Server.h"
#include <iostream>

const std::string SERVER_ADDRESS = "127.0.0.1";
const int SERVER_PORT = 8080;

int main() {
    std::cout << "Starting the server application..." << std::endl;
    
    // Create a server instance with a specific IP address and port
    HttpServer::Server server(SERVER_ADDRESS, SERVER_PORT);
    
    // Run the server
    server.run();
    
    return 0;
}