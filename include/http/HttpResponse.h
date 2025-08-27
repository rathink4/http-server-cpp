#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <utility>
#include <vector>


namespace HttpServer {
    class HttpResponse {
        public:
            HttpResponse() : status_code(200), status_message("OK") {}
            HttpResponse(int status_code, const std::string& statusMsg);
            ~HttpResponse() = default;

            // Getters
            int getStatusCode() const {return status_code;}
            const std::string& getStatusMessage() const {return status_message;}
            const std::string& getBody() const {return body;}
            const std::vector<std::pair<std::string, std::string>>& getHeaders() const {return headers;}

            // Setters
            void setStatusCode(int statusCode) { status_code = statusCode;}
            void setStatusMessage(const std::string& statusMsg) {status_message = statusMsg;}
            void setBody(const std::string& res_body) {body = res_body;}
            void addHeaders(const std::string& key, const std::string& value);

            // Static factory methods
            static HttpResponse ok(const std::string& body = "");
            static HttpResponse notFound(const std::string& message = "Not Found");
            static HttpResponse internalServerError(const std::string& message = "Internal Server Error");
            static HttpResponse badRequest(const std::string& message = "Bad Request");



        private:
            int status_code;
            std::string status_message;
            std::string body;
            std::vector<std::pair<std::string, std::string>> headers;
    };
}




#endif