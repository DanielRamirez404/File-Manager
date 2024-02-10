#include "terminal.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>
#include <string_view>
#include <map>
#include <algorithm>
#include <locale>
#include <string>

std::map<std::string_view, Terminal::Command> Terminal::commandMap 
{
    {"quit", Command::Quit},
    {"dir", Command::Dir},
};

void Terminal::run() const
{
    while(m_shouldRun)
    {
        printPath();
        executeCommand(getInput());
    }
}

void Terminal::printPath() const
{
    TreeDisplayer::printCurrentPath(m_tree);
    std::cout << "> ";
}

std::string Terminal::getInput() const
{
    std::string input{};
    std::cin >> input;
    std::transform
    (
        input.begin(), input.end(), input.begin(), [](char myChar)
        {
            return std::tolower(myChar);
        }
    );
    return input;
}

void Terminal::executeCommand(std::string_view command) const
{
    if (!commandMap.contains(command))
    {
        std::cout << "Invalid command. Please try again\n";
        return;
    }
    switch (commandMap[command])
    {
        case Command::Quit:
            m_shouldRun = false;
            break;
        case Command::Dir:
            TreeDisplayer::displayCurrent(m_tree);
            break;
    }
}
