#ifndef ROUTER_H
#define ROUTER_H

#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
#include <functional>


namespace HttpServer {
    
    using RouterHandler = std::function<HttpResponse(const HttpRequest&)>;
    
    class Router {
        public:
            Router();
            ~Router() = default;

            void addRoute(const std::string& method, const std::string& path, RouterHandler handler);
            void get(const std::string& path, RouterHandler handler);
            void post(const std::string& path, RouterHandler handler);
            void put(const std::string& path, RouterHandler handler);
            void del(const std::string& path, RouterHandler handler);

            HttpResponse route(const HttpRequest& request);

            void setNotFoundHandler(RouterHandler handler) {notFoundHandler = handler;}
            void setErrorHandler(RouterHandler handler) {errorHandler = handler;}

        private:
            // routes
            std::map<std::string, RouterHandler> routes; // key: "method:path"

            // Route handlers 
            RouterHandler notFoundHandler;
            RouterHandler errorHandler;

            // API methods
            HttpResponse handleCORS(const HttpRequest& req);
            HttpResponse handleTodoApiReqs(const HttpRequest& req);
            std::string extractIdFromPath(const std::string& path);

            // Utils
            std::string createRouteKey(const std::string& method, const std::string& path);

    };

    Router createDefaultRouter();

}

#endif