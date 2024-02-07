#include "directory tree.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>

int main()
{
    DirectoryTree tree {};
    tree.iterateToChild();
    tree.iterateToSibling();
    
    for (int i{0}; i < 3; i++)
    {
        TreeDisplayer::displayCurrent(tree);
        std::cout << "\n\n";
        tree.iterateBack();
    }
    
    return 0;
}
