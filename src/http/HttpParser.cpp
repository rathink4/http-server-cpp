#include "http/HttpParser.h"
#include <iostream>
#include <sstream>

namespace HttpServer {

    // Helper function to split a string by a delimiter
    std::vector<std::string> HttpParser::splitLines(const std::string& requestText) {
        std::vector<std::string> lines;
        std::string line;
        std::istringstream iss(requestText);
        while (std::getline(iss, line, '\n')) {
            // Remove carriage return character if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            lines.push_back(line);
        }
        return lines;
    }

    // Function to parse HTTP request from a string
    HttpRequest HttpParser::parseRequest(const std::string& rawRequest){
        HttpRequest req;
        
        if (rawRequest.empty()) {
            return req;
        }

        std::istringstream requestStream(rawRequest);
        std::string line;
        
        // 1. Parse the request line
        if (std::getline(requestStream, line)) {
            // Remove the trailing carriage return if it exists
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (!line.empty()) {
                if (!parseRequestLine(line, req)) {
                    std::cout << "Parsing Request Line failed" << std::endl;
                    return req;
                }
            } else {
                // Handle case where the first line is empty
                return req;
            }
        } else {
            // Handle case where requestStream is empty
            return req;
        }

        // 2. Collect header lines
        std::vector<std::string> headerLines;
        while (std::getline(requestStream, line) && line != "\r") {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (!line.empty()) {
                headerLines.push_back(line);
            }
        }

        // 3. Parse headers using the dedicated function
        parseHeaders(headerLines, req);

        // 4. Parse body (same as before)
        std::string body;
        std::string bodyLine;
        while (std::getline(requestStream, bodyLine)) {
            body += bodyLine;
            if (requestStream.peek() != EOF) {
                body += '\n';
            }
        }
        
        if (!body.empty() && body.back() == '\n') {
            body.pop_back();
        }

        if (!body.empty()) {
            req.setBody(body);
        }
        
        
        return req;


    }

    bool HttpParser::parseRequestLine(const std::string &line, HttpRequest &request) {
        std::cout << "Attempting to parse line: [" << line << "]" << std::endl;
        std::istringstream iss(line);
        std::string method, path, version;

        if (!(iss >> method >> path >> version)) {
            std::cerr << "Failed to extract method, path, and version." << std::endl;
            return false;
        }


        request.setMethod(method);
        request.setPath(path);
        request.setVersion(version);

        return true;
    }

    bool HttpParser::parseHeaders(const std::vector<std::string> &headerLines, HttpRequest &request) {
        for (const auto &line : headerLines) {
            std::pair<std::string, std::string> headerPair = parseHeaderLine(line);
            if (!headerPair.first.empty()) {
                request.addHeaders(headerPair.first, headerPair.second);
            }
        }
        return true; // Return true on success
    }

    
    std::pair<std::string, std::string> HttpParser::parseHeaderLine(const std::string& line) {
        size_t pos = line.find(":");
        if (pos == std::string::npos) {
            return {"", ""};
        }

        // Extract key and value strings
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Trim whitespace from both ends of the key and value
        size_t firstCharKey = key.find_first_not_of(" \t");
        size_t lastCharKey = key.find_last_not_of(" \t");
        if (std::string::npos != firstCharKey) {
            key = key.substr(firstCharKey, lastCharKey - firstCharKey + 1);
        } else {
            key.clear();
        }
        
        size_t firstCharValue = value.find_first_not_of(" \t");
        size_t lastCharValue = value.find_last_not_of(" \t");
        if (std::string::npos != firstCharValue) {
            value = value.substr(firstCharValue, lastCharValue - firstCharValue + 1);
        } else {
            value.clear();
        }
        
        return {key, value};

    }

    // Validation methods
    bool HttpParser::isValidMethod(const std::string& method) {
        return method == "GET" || method == "POST" || method == "PUT" || 
            method == "DELETE" || method == "HEAD" || method == "OPTIONS" ||
            method == "PATCH";
    }

    bool HttpParser::isValidPath(const std::string& path) {
        return !path.empty() && path[0] == '/';
    }

    bool HttpParser::isValidHttpVersion(const std::string& version) {
        return version == "HTTP/1.0" || version == "HTTP/1.1";
    }
    
}