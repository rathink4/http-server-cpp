#ifndef HTTP_FORMATTER_H
#define HTTP_FORMATTER_H

#include "HttpResponse.h"
#include <string>

namespace HttpServer {
    
    class HttpFormatter {
        public:
            static std::string formatHttpResponse(const HttpResponse& response);
    };
    
}

#endif