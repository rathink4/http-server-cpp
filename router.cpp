#import "httpRequestResponse.h"
#include <iterator>
#include <fstream>
#include <iostream>
#include <windows.h>

httpResponse getNotFoundResponse();
httpResponse getBadRequest(std::string msg);
httpResponse getHtmlPageResponse(std::string pageName);
std::string getHtmlContent(std::string path);

httpResponse route(httpRequest request) {
    auto path = request.path;
    httpResponse response;

    std::cout << "Routing request for path: " << path << std::endl;


    if (path == "/" || path == "home") {
        response = getHtmlPageResponse("home");
    } else if (path == "/about") {
        response = getHtmlPageResponse("about");
    } else {
        return getNotFoundResponse();
    }

    return response;
}

httpResponse getHtmlPageResponse(std::string pageName) {
    httpResponse response;
    response.headers.emplace_back("Content-Type", "text/html; charset=UTF-8");

    char cwd[1024];
    if (GetCurrentDirectoryA(1024, cwd)) {
        std::cout << "Current working directory: " << cwd << std::endl;
    }

    response.body = getHtmlContent("../../pages/" + pageName + ".html");
    if (response.body == "") {
        return getBadRequest("Page not found: " + pageName);
    }
    
    response.status_code = 200;
    response.status_message = "OK";

    return response;
}

std::string getHtmlContent(std::string path){
    std::ifstream htmlFile(path);
    if (htmlFile.is_open()){
        std::string content((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
        htmlFile.close();
        return content;
    }

    std::cerr << "Could not open file: " << path << std::endl;
    return "";

}

httpResponse getNotFoundResponse() {
    httpResponse res;
    res.status_code = 404;
    res.status_message = "Not Found";
    res.headers.emplace_back("Content-Type", "text/html; charset=UTF-8");
    res.body = R"(
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
)";
    return res;
}

httpResponse getBadRequest(std::string msg) {
    httpResponse res;
    res.status_code = 500;
    res.status_message = "Bad request!";
    res.body = R"(
<!DOCTYPE html>
<html>
<head>
    <title>500 - Server Error</title>
</head>
<body>
    <h1>500 - Internal Server Error</h1>
    <p>)" + msg + R"(</p>
</body>
</html>
)";
    return res;
}