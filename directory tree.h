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

        class Iterator
        {
            private:

                struct History
                {
                        std::list<const Node*> record{};
                        std::list<const Node*>::iterator current{};

                        History(const Node* node);
                        void add(const Node* node);
                        void reset(const Node* currentNode);
                };

                mutable const Node* m_pointer{};
                mutable History m_history{ m_pointer };

            public:

                Iterator(const Node* pointer);
                Iterator(const Iterator&) = delete;
                void operator=(const Iterator&) = delete;

                const Node* get() const;
                void toNode(const Node* node) const;
                void toSibling() const;
                void toChild() const;
                void toRoot() const;
                void back() const;
                void forward() const;

                friend class TreeDisplayer;
        };

        std::unique_ptr<Node> m_root{};
        Iterator m_iterator{ m_root.get() };

        void addChildren(Node* node);
        const Node* findChildPath(const Node* node, const fs::path& path) const;

    public:

        DirectoryTree(const DirectoryTree&) = delete;
        void operator=(const DirectoryTree&) = delete;
        DirectoryTree();
        explicit DirectoryTree(const fs::path& path);

        const Node* findPath(const fs::path& path) const;
        void iterateToPath(const fs::path& path) const;
        const Iterator& iterator() const;

        friend class TreeDisplayer;
};
