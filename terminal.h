#pragma once
#include "directory tree.h"
#include <map>
#include <string_view>

class Terminal 
{
    private:

        enum Command {
            Quit,
            Dir
        };

        static std::map<std::string_view, Command> commandMap;

        mutable bool m_shouldRun{ true };
        DirectoryTree m_tree{};

        void printPath() const;
        std::string getInput() const;
        void executeCommand(std::string_view command) const;
        
    public:

        Terminal() = default;
        Terminal(const Terminal&) = delete;
        void operator=(const Terminal&) = delete;
        void run() const;
};
