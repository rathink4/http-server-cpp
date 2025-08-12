#include "httpRequestResponse.h"
#include <iostream>
#include <sstream>

// Helper function to split a string by a delimiter
std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    
    return result;
}

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Function to parse HTTP request from a string
httpRequest parseHttpRequest(const std::string& message) {
    httpRequest req;
    
    // Split by \r\n first to handle proper HTTP line endings
    std::vector<std::string> lines = split(message, "\r\n");
    
    if (lines.empty()) return req;
    
    // Parse the request line (first line: METHOD PATH HTTP/VERSION)
    std::istringstream requestLine(lines[0]);
    std::string httpVersion;
    requestLine >> req.method >> req.path >> httpVersion;
    
    // Parse headers
    size_t i = 1;
    for (; i < lines.size() && !lines[i].empty(); ++i) {
        size_t colonPos = lines[i].find(':');
        if (colonPos != std::string::npos) {
            std::string key = trim(lines[i].substr(0, colonPos));
            std::string value = trim(lines[i].substr(colonPos + 1));
            req.headers[key] = value;
        }
    }
    
    // Parse body (everything after the empty line)
    if (i + 1 < lines.size()) {
        std::string body;
        for (size_t j = i + 1; j < lines.size(); ++j) {
            if (!body.empty()) body += "\r\n";
            body += lines[j];
        }
        req.body = body;
    }
    
    return req;
}

// Function to format HTTP response as a string
std::string formatHttpResponse(const httpResponse& response) {
    std::ostringstream oss;
    
    // Status line
    oss << "HTTP/1.1 " << response.status_code << " " << response.status_message << "\r\n";
    
    // Headers
    for (const auto& header : response.headers) {
        oss << header.first << ": " << header.second << "\r\n";
    }
    
    // Content-Length header (important for proper HTTP response)
    bool hasContentLength = false;
    for (const auto& header : response.headers) {
        if (header.first == "Content-Length") {
            hasContentLength = true;
            break;
        }
    }
    
    if (!hasContentLength) {
        oss << "Content-Length: " << response.body.length() << "\r\n";
    }
    
    // Empty line to separate headers from body
    oss << "\r\n";
    
    // Body
    oss << response.body;
    
    return oss.str();
}