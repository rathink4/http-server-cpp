#ifndef TODO_H
#define TODO_H

#include <ctime>
#include <string>
#include <vector>

namespace HttpServer {
    // Todo data template
    struct Todo {
        int id;
        std::string text;
        bool completed;
        std::string createdAt;

        Todo() : id(0), completed(false) {}
        Todo(int todoId, std::string todoText, bool isCompleted=false) : id(todoId), text(todoText), completed(isCompleted) {
            // Generate the current time
            time_t now = time(0);
            createdAt = ctime(&now);
            // Remove newline from the timestamp
            if (!createdAt.empty() && createdAt.back() == '\n') {
                createdAt.pop_back();
            }

        }

    };

    class TodoStorage {
        private:
            std::vector<Todo> todos;
            int s_id;
        
        public:
            TodoStorage();
            ~TodoStorage();

            // Singleton instance
            static TodoStorage& getInstance();

            // CRUD operations
            std::vector<Todo> getAllTodos() const;
            Todo* getTodoById(int _id);
            int addTodo(const std::string& text);
            bool updateTodo(int _id, const std::string _text, const bool _completed);
            bool deleteTodo(int _id);

            // JSON conversion
            std::string todoToJson(const Todo& todo) const;
            std::string todosToJson(const std::vector<Todo>& todo) const;
            Todo parseJsonTodo(std::string& jsonStr) const;

            int getNextId();
            size_t getTodoCount() const;
            void clear();

    };
}

#endif