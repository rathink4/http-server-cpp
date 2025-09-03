#include "utils/TodoStorage.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <vector>

namespace HttpServer {
    
    TodoStorage::TodoStorage() : s_id(1) {
        TodoStorage::addTodo("Learn C++ HTTP Server Development");
        TodoStorage::addTodo("Implement REST API endpoints");
    }

    TodoStorage::~TodoStorage() {}

    // Singleton Instance
    TodoStorage& TodoStorage::getInstance() {
        static TodoStorage instance;
        return instance;
    }

    // CRUD operations
    std::vector<Todo> TodoStorage::getAllTodos() const {
        return todos;
    }

    Todo* TodoStorage::getTodoById(int id) {
        auto it = std::find_if(todos.begin(), todos.end(), [id](const Todo& todo) {return todo.id == id;});

        if (it != todos.end()) {
            return &(*it);
        }

        return nullptr;
    }

    int TodoStorage::addTodo(const std::string& text) {
        Todo newTodo(s_id++, text);
        todos.push_back(newTodo);
        std::cout << "Added todo with ID: " << newTodo.id << ", Text: " << text << std::endl;
        return newTodo.id;
    }

    bool TodoStorage::updateTodo(int id, const std::string text, bool isCompleted) {
        Todo* todo = getTodoById(id);
        if (todo) {
            todo->text = text;
            todo->completed = isCompleted;
            std::cout << "Updated Todo ID: " << todo->id << ", Text: " << todo->text << ", completion status: " << todo->completed << std::endl;
            return true;
        }

        std::cout << "Failed to update Todo ID: " << todo->id;
        return false;
    }

    bool TodoStorage::deleteTodo(int id) {
        auto it = std::find_if(todos.begin(), todos.end(), [id](const Todo& todo) {return todo.id == id;});

        if (it != todos.end()) {
            std::cout << "Deleted Todo ID: " << id;
            todos.erase(it);
            return true;
        }

        std::cout << "Failed to delete Todo ID: " << id;
        return false;
    }


    // JSON operations
    std::string TodoStorage::todoToJson(const Todo& todo) const {
        std::ostringstream json;
        json << "{"
            << "\"id\":" << todo.id << ","
            << "\"text\":" << todo.text << ","
            << "\"completed\":" << (todo.completed ? "true" : "false") << ","
            << "\"createdAt\":" << todo.createdAt << "\""
            << "}";
        
        return json.str();
    }

    std::string TodoStorage::todosToJson(const std::vector<Todo>& _todos) const {
        std::ostringstream json;
        json << "[";

        for (size_t i = 0; i < _todos.size(); ++i) {
            json << todoToJson(_todos[i]);
            if (i < _todos.size()-1) {
                json << ",";
            }
        }
        json << "]";

        return json.str();
    }

    Todo TodoStorage::parseJsonTodo(std::string& jsonStr) const{
        Todo todo;

        // Extract id
        size_t idPos = jsonStr.find("\"id\":");
        if (idPos != std::string::npos) {
            idPos += 5; // Move past "\"id\":"
            while (idPos < jsonStr.length() && (jsonStr[idPos] == ' ' || jsonStr[idPos] == ':')) idPos++;
            size_t idEnd = jsonStr.find_first_of(",}", idPos);
            if (idEnd != std::string::npos) {
                std::string idStr = jsonStr.substr(idPos, idEnd - idPos);
                todo.id = std::stoi(idStr);
            }
        }
        
        // Extract text
        size_t textPos = jsonStr.find("\"text\":\"");
        if (textPos != std::string::npos) {
            textPos += 8; // Move past "\"text\":\""
            size_t textEnd = jsonStr.find("\"", textPos);
            if (textEnd != std::string::npos) {
                todo.text = jsonStr.substr(textPos, textEnd - textPos);
            }
        }
        
        // Extract completed
        size_t completedPos = jsonStr.find("\"completed\":");
        if (completedPos != std::string::npos) {
            completedPos += 12; // Move past "\"completed\":"
            while (completedPos < jsonStr.length() && (completedPos < jsonStr.length() && jsonStr[completedPos] == ' ')) completedPos++;
            if (completedPos < jsonStr.length()) {
                todo.completed = (jsonStr.substr(completedPos, 4) == "true");
            }
        }
        
        // Set creation time
        time_t now = time(0);
        todo.createdAt = ctime(&now);
        if (!todo.createdAt.empty() && todo.createdAt.back() == '\n') {
            todo.createdAt.pop_back();
        }
        
        return todo;
    }

}