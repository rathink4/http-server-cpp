#include "http/HttpResponse.h"
#include "http/HttpFormatter.h"
#include <sstream>

namespace HttpServer {

    std::string HttpFormatter::formatHttpResponse(const HttpResponse& response) {
        std::ostringstream oss;
        
        // Status line
        oss << "HTTP/1.1 " << response.getStatusCode() 
            << " " << response.getStatusMessage() << "\r\n";
        
        // Headers
        for (const auto& header : response.getHeaders()) {
            oss << header.first << ": " << header.second << "\r\n";
        }
        
        // Content-Length header (automatically add if not present)
        bool hasContentLength = false;
        for (const auto& header : response.getHeaders()) {
            if (header.first == "Content-Length") {
                hasContentLength = true;
                break;
            }
        }
        
        if (!hasContentLength) {
            oss << "Content-Length: " << response.getBody().length() << "\r\n";
        }
        
        // Empty line to separate headers from body
        oss << "\r\n";
        
        // Body
        oss << response.getBody();
        
        return oss.str();
    }

}