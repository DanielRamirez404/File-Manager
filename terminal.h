#pragma once
#include "directory tree.h"

class Terminal 
{
    private:

        mutable bool m_shouldRun{ true };
        DirectoryTree m_tree{};
        
    public:

        Terminal() = default;
        Terminal(const Terminal&) = delete;
        void operator=(const Terminal&) = delete;
        void run() const;
};
