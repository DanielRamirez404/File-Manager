#include "directory tree.h"
#include <filesystem>
#include <iostream>

int main()
{
    DirectoryTree tree { fs::current_path(), 2 };
    return 0;
}
