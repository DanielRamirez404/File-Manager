#include "directory tree.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>

int main()
{
    DirectoryTree tree {};
    tree.iterator().toChild();
    tree.iterator().toSibling();
    
    for (int i{0}; i < 3; i++)
    {
        TreeDisplayer::displayCurrent(tree);
        std::cout << "\n\n";
        tree.iterator().back();
    }
    
    return 0;
}
