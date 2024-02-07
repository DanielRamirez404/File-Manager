#pragma once
#include <memory>
#include <string>
#include <list>
#include <filesystem>

namespace fs = std::filesystem;

class DirectoryTree
{
    private:

        struct Node
        {
            fs::path path{};
            std::unique_ptr<Node> firstChild { nullptr };
            std::unique_ptr<Node> nextSibling { nullptr };
        };

        struct History
        {
            std::list<const Node*> record{};
            std::list<const Node*>::iterator current{};
            void add(const Node* node);
            void deleteLast();
            void reset();
        };

        std::unique_ptr<Node> m_root{};
        const Node* m_iterator{};
        History m_history{};

        void addChildren(Node* node);
        const Node* findChildPath(const Node* node, const fs::path& path) const;
        void iterateToNode(const Node* node);

    public:

        DirectoryTree(const DirectoryTree&) = delete;
        void operator=(const DirectoryTree&) = delete;
        DirectoryTree();
        explicit DirectoryTree(const fs::path& path);

        const Node* findPath(const fs::path& path) const;

        void iterateToSibling();
        void iterateToChild();
        void iterateBack();
        void iterateForward();
        void resetIterator();
        void iterateToPath(const fs::path& path);

        friend class TreeDisplayer;
};
