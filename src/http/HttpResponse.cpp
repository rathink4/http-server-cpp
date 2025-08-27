#include "http/HttpResponse.h"

namespace HttpServer {

    HttpResponse::HttpResponse(int _status_code, const std::string& statusMsg) : status_code(_status_code), status_message(statusMsg) {
    }

    void HttpResponse::addHeaders(const std::string& key, const std::string& value) {
        headers.emplace_back(key, value);
    }

    HttpResponse HttpResponse::ok(const std::string& body) {
        HttpResponse response(200, "OK");
        response.setBody(body);
        return response;
    }

    HttpResponse HttpResponse::notFound(const std::string& message) {
        HttpResponse response(404, "Not Found");
        response.setBody(message);
        return response;
    }

    HttpResponse HttpResponse::badRequest(const std::string& message) {
        HttpResponse response(400, "Bad Request");
        response.setBody(message);
        return response;
    }

    HttpResponse HttpResponse::internalServerError(const std::string& message) {
        HttpResponse response(500, "Server Error");
        response.setBody(message);
        return response;
    }
}