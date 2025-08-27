#include "server/Router.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <iterator>

namespace HttpServer {
    Router::Router() {
        notFoundHandler = [](const HttpRequest& req) -> HttpResponse {
            return HttpResponse::notFound(R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>404 - Not Found</title>
            </head>
            <body>
                <h1>404 - Page Not Found</h1>
                <p>The requested page could not be found.</p>
            </body>
            </html>
            )");
        };

        errorHandler = [](const HttpRequest& req) -> HttpResponse {
            return HttpResponse::internalServerError(R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>500 - Server Error</title>
            </head>
            <body>
                <h1>500 - Internal Server Error</h1>
                <p>An internal server error occurred.</p>
            </body>
            </html>
            )");
        };
    }

    void Router::addRoute(const std::string& method, const std::string& path, RouterHandler handler) {
        std::string key = createRouteKey(method, path);
        routes[key] = handler;
    }

    void Router::get(const std::string& path, RouterHandler handler) {
        addRoute("GET", path, handler);
    }

    void Router::post(const std::string& path, RouterHandler handler) {
        addRoute("POST", path, handler);
    }

    void Router::del(const std::string& path, RouterHandler handler) {
        addRoute("DELETE", path, handler);
    }

    HttpResponse Router::route(const HttpRequest& request) {
        std::cout << "Routing request for path: " << request.getPath() << std::endl;

        try {
            std::string key = createRouteKey(request.getMethod(), request.getPath());

            auto it = routes.find(key);

            if (it != routes.end()) {
                return it->second(request);
            }

            return notFoundHandler(request);

        } catch (const std::exception& e) {
            std::cerr << "Error in routing: " << e.what() << std::endl;
            return errorHandler(request);
        }

    }

    std::string Router::createRouteKey(const std::string& method, const std::string& path) {
        return method + ":" + path;
    }


    // Utility functions for HTML handling
    namespace {
        HttpResponse getHtmlPageResponse(const std::string& pageName) {
            HttpResponse response;
            response.addHeaders("Content-Type", "text/html; charset=UTF-8");


            std::string htmlPath = "../../pages/" + pageName + ".html";
            std::ifstream htmlFile(htmlPath);

            if (htmlFile.is_open()) {
                std::string content((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
                htmlFile.close();

                response.setBody(content);
                response.setStatusCode(200);
                response.setStatusMessage("OK");

                return response;
            } else {
                std::cerr << "Could not open file: " << htmlPath << std::endl;
                
                return HttpResponse::internalServerError("Page not found " + pageName);

            }

        }

        HttpResponse homeHandler(const HttpRequest& req) {
            return getHtmlPageResponse("home");
        }

        HttpResponse aboutHandler(const HttpRequest& req) {
            return getHtmlPageResponse("about");
        }
    
    }

    // Factory method to create router with default config
    Router createDefaultRouter() {
        Router router;

        router.get("/", homeHandler);
        router.get("/home", homeHandler);
        router.get("/about", aboutHandler);

        return router;
    }


}