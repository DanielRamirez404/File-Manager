#include "terminal.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>
#include <string>

void Terminal::run() const
{
    while(m_shouldRun)
    {
        TreeDisplayer::printCurrentPath(m_tree);
        std::cout << "> ";
        std::string input{};
        std::cin >> input;

        if (input == "quit")
            m_shouldRun = false;
    }
}
