#include "directory tree.h"
#include "tree displayer.h"
#include <filesystem>
#include <iostream>

int main()
{
    DirectoryTree tree { fs::current_path(), 2 };
    TreeDisplayer::display(tree);
    return 0;
}
