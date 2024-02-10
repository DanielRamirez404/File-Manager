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
    {"cd", Command::Cd},
    {"cd root", Command::CdToRoot},
    {"cd child", Command::CdToChild},
    {"cd sibling", Command::CdToSibling},
    {"cd -", Command::CdBack},
    {"cd +", Command::CdForward}
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
    std::getline(std::cin >> std::ws, input);
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
        std::cout << "Invalid command. Please try again\n\n";
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
        case Command::Cd:
            TreeDisplayer::printCurrentPath(m_tree);
            std::cout << '\n';
            break;
        case Command::CdToRoot:
            m_tree.iterator().toRoot();
            break;
        case Command::CdToChild:
            m_tree.iterator().toChild();
            break;
        case Command::CdToSibling:
            m_tree.iterator().toSibling();
            break;
        case Command::CdBack:
            m_tree.iterator().back();
            break;
        case Command::CdForward:
            m_tree.iterator().forward();
            break;
    }
    std::cout << '\n';
}
