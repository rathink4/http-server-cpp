# HTTP Web Server in Windows using C++

An easy-to-understand HTTP Web Server for all the Windows users that want to work with C++ and learn about how to build an HTTP Web Server from scratch.


<div align="center">
    <img height=640 width=1280 align="center" src="/images/home.JPG">
    <br><br>
    <img height=640 width=1280 align="center" src="/images/about.JPG">
</div>


# Contents

- [Why?](#why)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [TODO](#todo)

### Why?

I wanted to build an HTTP Web server from scratch because:

- Wanted to learn how to build one from scratch.
- Didn't find a good reference project that did it for `Windows`.
- Implement socket programming in `C++`.


### Project Structure
```shell
http-server-cpp/
├── CMakeLists.txt
├── README.md
├── build/                          # Build output directory
├── include/                        # Header files
│   ├── http/                       # HTTP-related headers
│   │   ├── HttpRequest.h
│   │   ├── HttpResponse.h
│   │   ├── HttpParser.h
│   │   └── HttpFormatter.h
│   └── server/                     # Server-related headers
│   │   ├── Server.h
│   │   └── Router.h
├── src/                            # Source files
│   ├── http/                       # HTTP implementation
│   │   ├── HttpRequest.cpp
│   │   ├── HttpResponse.cpp
│   │   ├── HttpParser.cpp
│   │   └── HttpFormatter.cpp
│   ├── server/                     # Server implementation
│   │   ├── Server.cpp
│   │   └── Router.cpp
│   └── main.cpp                    # Application entry point
└── pages/                          # Static web content
    ├── index.html
    ├── about.html
    └── todo.html
```

### Prerequisites

---

1. Install CMake
2. Make sure you have Ws2_32.lib library files



### Installation

---

#### Step 1: Clone the Repo

```bash
$ git clone https://github.com/rathink4/http-server-cpp.git
$ cd http-server-cpp

```

#### Step 2: Build the Project

Create the build directory and navigate into it:

```bash
$ mkdir build
$ cd build

```

Run CMake to configure the project

```bash
$ cmake ..

```

Build the application

```bash
$ cmake --build .

```

#### Step 3: Run the server

```bash
$ cd bin
$ ./HttpServerCpp

```

### TODO

---

- [x] Document the README.md file.
- [x] Implement TCP Server-Client connection.
- [x] Implement HTTP Server-Client connection.
- [x] Refactored code for extensibility
- [ ] Add GET, POST, PUT, DELETE functionality in the web server.
- [ ] Handle multiple client connections requests.