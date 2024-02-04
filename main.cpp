#include "directory tree.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>

int main()
{
    DirectoryTree tree { fs::current_path(), 2 };
    TreeDisplayer::displayFull(tree);
    std::cout << "\n\n";
    tree.iterateToChild();
    tree.iterateToSibling();
    TreeDisplayer::displayCurrent(tree);
    std::cout << "\n\n";
    TreeDisplayer::displayHistory(tree);
    return 0;
}
