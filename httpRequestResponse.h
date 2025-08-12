#ifndef HTTP_REQ_RES_H
#define HTTP_REQ_RES_H

#include <string>
#include <map>
#include <vector>

struct httpRequest {
    std::string method; // HTTP method (eg. GET, POST, DELETE)
    std::string path;   // Request path
    std::map<std::string, std::string> headers;     // Request headers
    std::string body;   // Request body (optional)
};



struct httpResponse {
    int status_code;    // Response code (200, 400, 500)
    std::string status_message = "OK"; // Response status (OK, NOT Found)
    std::vector<std::pair<std::string, std::string>> headers;   // Response headers
    std::string body;   // Response body
};


httpRequest parseHttpRequest(const std::string& message);
httpResponse parseHttpResponse(const std::string& message);
httpResponse route(httpRequest request);
std::string formatHttpResponse(const httpResponse& response);

#endif