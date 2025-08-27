#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "HttpRequest.h"
#include <string>
#include <utility>
#include <vector>

namespace HttpServer {
    class HttpParser {
        public:
            HttpParser() = default;
            ~HttpParser() = default;

            // Parse HTTP req from string
            static HttpRequest parseRequest(const std::string& rawRequest);

            // Parse each component of the request
            static bool parseRequestLine(const std::string& line, HttpRequest& request);
            static bool parseBody(const std::string& rawRequest);
            static bool parseHeaders(const std::vector<std::string>& headerLines, HttpRequest& request);

            // Sanity check functions
            static bool isValidMethod(const std::string& method);
            static bool isValidPath(const std::string& path);
            static bool isValidHttpVersion(const std::string& version);

            // Helper util functions
            static std::vector<std::string> splitLines(const std::string& requestText);
            static std::pair<std::string, std::string> parseHeaderLine(const std::string& line);
    };
}

#endif