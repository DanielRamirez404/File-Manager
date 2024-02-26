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
                        std::list<const Node*>::iterator currentNode{};

                        History() = default;
                        History(const Node* node);
                        const Node* current();
                        void setCurrentToLast();
                        void add(const Node* node);
                        void reset();
                        void deleteUntilCurrent();
                        void deleteAfterCurrent();
                };

                mutable History m_history{};
                mutable DirectoryTree* m_this_tree{};

                void toNonExistentNode(const fs::path& path) const;

            public:

                Iterator(const Node* pointer, DirectoryTree* currentTree);
                Iterator(const Iterator&) = delete;
                void operator=(const Iterator&) = delete;

                const Node* get() const;
                void toPath(const fs::path& path) const;
                void toNode(const Node* node) const;
                void toSibling() const;
                void toChild() const;
                void toParent() const;
                void toRoot() const;
                void back() const;
                void forward() const;

                friend class TreeDisplayer;
        };

        std::unique_ptr<Node> m_root{};
        Iterator m_iterator{ m_root.get(), this };

        void addChildren(Node* parent, std::unique_ptr<Node>* existingChild = nullptr);
        void addRootParent();
        const Node* findChild(const Node* node, const fs::path& path) const;
        const Node* findParent(const Node* node) const;

    public:

        DirectoryTree(const DirectoryTree&) = delete;
        void operator=(const DirectoryTree&) = delete;
        DirectoryTree();
        explicit DirectoryTree(const fs::path& path);

        const Node* findPath(const fs::path& path) const;
        const Iterator& iterator() const;

        friend class TreeDisplayer;
};
