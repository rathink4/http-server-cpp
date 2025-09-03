#include "server/Router.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "utils/TodoStorage.h"
#include <iostream>
#include <string>
#include <vector>
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

    void Router::put(const std::string& path, RouterHandler handler) {
        addRoute("PUT", path, handler);
    }

    void Router::del(const std::string& path, RouterHandler handler) {
        addRoute("DELETE", path, handler);
    }

    HttpResponse Router::route(const HttpRequest& request) {
        std::cout << "Routing request for path: " << request.getPath() << std::endl;

        try {

            // Prechecks for api based logic

            // CORS request check
            if (request.getMethod() == "OPTIONS") {
                return handleCORS(request);
            }

            // Check for API routes
            if (request.getPath().find("/api/todos") == 0) {
                return handleTodoApiReqs(request);
            }

            // Static page serving routing logic
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

    HttpResponse Router::handleCORS(const HttpRequest& req) {
        HttpResponse res;
        res.setStatusCode(200);
        res.setStatusMessage("OK");

        res.addHeaders("Access-Control-Allow-Origin", "*");
        res.addHeaders("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.addHeaders("Access-Control-Allow-Headers", "Content-Type");
        res.addHeaders("Access-Control-Max-Age", "86400");
        res.setBody("");

        return res;
    }

    HttpResponse Router::handleTodoApiReqs(const HttpRequest& req) {
        std::cout << "Handling Todo API Request: " << req.getMethod() << " " << req.getPath() << std::endl;
               
        HttpResponse res;
        TodoStorage& storage = TodoStorage::getInstance();

        // Headers for CORS
        res.addHeaders("Access-Control-Allow-Origin", "*");
        res.addHeaders("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.addHeaders("Access-Control-Allow-Headers", "Content-Type");
        res.addHeaders("Content-Type", "application/json");

        try {
            if ((req.getMethod() == "GET" && req.getPath() == "/api/todos")) {
                // Get /api/todos -> Get all todos
                std::vector<Todo> todos = storage.getAllTodos();
                std::string jsonResponse = storage.todosToJson(todos);
                
                res.setStatusCode(200);
                res.setStatusMessage("OK");
                res.setBody(jsonResponse);

            } else if ((req.getMethod() == "POST" && req.getPath() == "/api/todos")) {
                // Post /api/todos -> Create new todo
                std::string reqBody = req.getBody();
                
                std::cout << "POST request body: " << reqBody << std::endl;

                if (reqBody.empty()) {
                    res.badRequest("{\"error:\":\"Request body is required\"}");
                    return res;
                }

                Todo newTodo = storage.parseJsonTodo(reqBody);
                int todoId = storage.addTodo(newTodo.text);

                // Get the created todo to return
                Todo* createdTodo = storage.getTodoById(todoId);
                if (createdTodo) {
                    std::string jsonResponse = storage.todoToJson(*createdTodo);
                    res.setStatusCode(200);
                    res.setStatusMessage("Created");
                    res.setBody(jsonResponse);
                } else {
                    res.internalServerError("{\"error\":\"Failed to create todo\"}");
                }

            } else if ((req.getMethod() == "PUT" && req.getPath().find("/api/todos/") == 0)) {
                // Put - /api/todos/{id} - Update todo
                std::string idStr = extractIdFromPath(req.getPath());

                if (idStr.empty()) {
                    res.badRequest("{\"error:\":\"Todo id not found\"}");
                    return res;
                }

                int todoId = std::stoi(idStr);
                std::string reqBody = req.getBody();
                std::cout << "PUT request body: " << reqBody << std::endl;

                if (reqBody.empty()) {
                    res.badRequest("{\"error:\":\"Request body is required\"}");
                    return res;
                }

                Todo updateTodo = storage.parseJsonTodo(reqBody);
                bool success = storage.updateTodo(todoId, updateTodo.text, updateTodo.completed);

                if (success) {
                    Todo* todo = storage.getTodoById(todoId);
                    if (todo) {
                        std::string jsonResponse = storage.todoToJson(*todo);
                        res.ok(jsonResponse);
                    }
                } else {
                    res.notFound("{\"error\":\"Todo not found\"}");
                }


            } else if ((req.getMethod() == "DELETE" && req.getPath().find("/api/todos/") == 0)) {
                // delete - /api/todos/{id} - Delete todo
                std::string idStr = extractIdFromPath(req.getPath());

                if (idStr.empty()) {
                    res.badRequest("{\"error:\":\"Todo id not found\"}");
                    return res;
                }

                int todoId = std::stoi(idStr);
                bool success = storage.deleteTodo(todoId);

                if (success) {
                    res.ok("{\"message\":\"Todo deleted successfully\"}");
                } else {
                    res.notFound("{\"error\":\"Todo not found\"}");
                }

            } else {
                res.setStatusCode(404);
                res.setStatusMessage("Not Found");
                res.setBody("{\"error\":\"API endpoint not found\"}");
                return notFoundHandler(req);
            }

        } catch (const std::exception& e) {
            std::cerr << "Error in handling TODOs: " << e.what() << std::endl;
            return errorHandler(req);
        }

        return res;
    }

    std::string Router::extractIdFromPath(const std::string& path) {
        // Extract ID from path like "/api/todos/123"
        size_t lastSlash = path.find_last_of('/');
        if (lastSlash != std::string::npos && lastSlash < path.length() - 1) {
            std::string idStr = path.substr(lastSlash + 1);
            // Validate that it's a number
            if (std::all_of(idStr.begin(), idStr.end(), ::isdigit)) {
                return idStr;
            }
        }
        return "";
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

        HttpResponse todoHandler(const HttpRequest& req) {
            return getHtmlPageResponse("todo");
        }
    
    }

    // Factory method to create router with default config
    Router createDefaultRouter() {
        Router router;

        router.get("/", homeHandler);
        router.get("/home", homeHandler);
        router.get("/about", aboutHandler);
        router.get("/todo", todoHandler);


        return router;
    }


}