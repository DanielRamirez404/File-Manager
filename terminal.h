#pragma once
#include "directory tree.h"
#include <map>
#include <string_view>
#include <vector>

class Terminal 
{
    private:

        enum Command 
        {
            Quit,
            Dir,
            Cd,
            CdToRoot,
            CdToChild,
            CdToSibling,
            CdBack,
            CdForward
        };

        static std::map<std::string_view, Command> commandMap;

        mutable bool m_shouldRun{ true };
        DirectoryTree m_tree{};

        void printPath() const;
        std::string getInput() const;
        void executeCommand(std::string_view command) const;
        std::vector<std::string_view> parseCommand(std::string_view command) const;
        int countWords(std::string_view command) const;
        std::string_view getFirstWord(std::string_view command) const;
        
    public:

        Terminal() = default;
        Terminal(const Terminal&) = delete;
        void operator=(const Terminal&) = delete;
        void run() const;
};
