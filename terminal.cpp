#include "terminal.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>
#include <vector>
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

    const auto commands { parseCommand(command) };
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

std::vector<std::string_view> Terminal::parseCommand(std::string_view command) const
{
    const auto wordCounter { static_cast<size_t>( countWords(command) ) };
    std::vector<std::string_view> words{ wordCounter };

    while (!command.empty())
    {

        if (command[0] == ' ')
        {
            command.remove_prefix(1);
            continue;
        }

        struct WordParser
        {
            bool isPath{ false };
            size_t index{0};
            std::string_view word{};
        } parser;

        while (command[parser.index] != ' ' || parser.isPath)
        {
            if (command[parser.index] == '\"')
            {
                parser.isPath = !parser.isPath;
            }
            ++parser.index;
        }

        parser.word = command.substr(0, parser.index);

        words.push_back(parser.word);

        command.remove_prefix(parser.word.size());
    }

    return words;
}

int Terminal::countWords(std::string_view command) const
{
    bool isPath { false };
    return std::count_if
    (
        command.begin(), command.end(), [&isPath](char myChar)
        {
            if (myChar == '\"') 
            {
                isPath = !isPath;
                return false;
            }
            return (!isPath) && (myChar == ' ');
        }
    );
}
