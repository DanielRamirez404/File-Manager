#include "directory tree.h"
#include <filesystem>
#include <iostream>

int main()
{
    DirectoryTree tree { fs::current_path(), 1 };
    return 0;
}
