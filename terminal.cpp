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
};

std::map<std::string_view, Terminal::CdCommand> Terminal::cdCommandMap
{
    {"root", CdCommand::root},
    {"child", CdCommand::child},
    {"sibling", CdCommand::sibling},
    {"-", CdCommand::back},
    {"+", CdCommand::forward},
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

    while (input[0] == ' ')
        input.erase(input.begin());

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
    const auto wordList { parseCommand(command) };

    std::string_view firstCommand { wordList[0] };

    if (!commandMap.contains(firstCommand))
    {
        std::cout << "Invalid command. Please try again\n\n";
        return;
    }

    switch (commandMap[firstCommand])
    {
        case Command::Quit:
            m_shouldRun = false;
            break;
        case Command::Dir:
            TreeDisplayer::displayCurrent(m_tree);
            break;
        case Command::Cd:
            executeCdCommand(wordList);     
            break;
    }

    std::cout << '\n';
}

void Terminal::executeCdCommand(const std::vector<std::string_view>& wordList) const
{
    if (wordList.size() < 2)
    {
        TreeDisplayer::printCurrentPath(m_tree);
        std::cout << '\n';
        return;
    }

    std::string_view cdCommand { wordList[1] };

    if (!cdCommandMap.contains(cdCommand))
    {
    
        if (cdCommand[0] == '\"')
        {
            cdCommand.remove_prefix(1);
            cdCommand.remove_suffix(1);
        }

        const fs::path path{ cdCommand };

        if (fs::exists(path))
            m_tree.iterator().toPath(path);
        else
            std::cout << "Invalid command. Please try again\n";

        return;
    }

    switch (cdCommandMap[cdCommand])
    {
        case CdCommand::root:
            m_tree.iterator().toRoot();
            break;
        case CdCommand::child:
            m_tree.iterator().toChild();
            break;
        case CdCommand::sibling:
            m_tree.iterator().toSibling();
            break;
        case CdCommand::back:
            m_tree.iterator().back();
            break;
        case CdCommand::forward:
            m_tree.iterator().forward();
            break;
    }
}

std::vector<std::string_view> Terminal::parseCommand(std::string_view command) const
{
    const auto wordCounter { static_cast<size_t>( countWords(command) ) };

    std::vector<std::string_view> words{};
    words.reserve(wordCounter);

    while (!command.empty())
    {
        while (command[0] == ' ')
            command.remove_prefix(1);

        std::string_view foundWord{ getFirstWord(command) };
        words.push_back(foundWord);
        
        command.remove_prefix(foundWord.size());
    }

    return words;
}

int Terminal::countWords(std::string_view command) const
{
    bool isPath { false };
    return 1 + std::count_if
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

std::string_view Terminal::getFirstWord(std::string_view command) const
{
    bool isPath{ false };

    for (size_t i{0}; i + 1 < command.size(); ++i)
    {
        if (command[i] == '\"')
            isPath = !isPath;

        if (command[i + 1] == ' ' && !isPath)
            return command.substr(0, i + 1);  
    }

    return command;
}
