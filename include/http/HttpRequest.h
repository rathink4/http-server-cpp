#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <map>
#include <string>

namespace HttpServer {
    class HttpRequest {
        public:
            HttpRequest() = default;
            ~HttpRequest() = default;

            // Getters
            const std::string& getMethod() const {return method;}
            const std::string& getPath() const {return path;}
            const std::string& getVersion() const {return version;}
            const std::string& getBody() const {return body;}
            const std::map<std::string, std::string>& getHeaders() const {return headers;}

            // Setters
            void setMethod(const std::string& req_method) {method = req_method;}
            void setPath(const std::string& req_path) {path = req_path;}
            void setVersion(const std::string& _version) {version = _version;}
            void setBody(const std::string& req_body) {body = req_body;}
            void addHeaders(const std::string& key, const std::string& value);


        private:
            std::string method;
            std::string path;
            std::string version;
            std::string body;
            std::map<std::string, std::string> headers;
    };
}

#endif