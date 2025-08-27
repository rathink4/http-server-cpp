#include "http/HttpRequest.h"

namespace HttpServer {

    void HttpRequest::addHeaders(const std::string& key, const std::string& value) {
        headers[key] = value;
    }

}