#pragma once
#include <vector>
#include <algorithm>
#include <chrono>

template<typename KeyType, typename ValueType>
class BaseTable;

template<typename KeyType, typename ValueType>
class SimpleTable;

template<typename KeyType, typename ValueType>
class SortTable;


template<typename KeyType, typename ValueType>
class Iterator : public std::iterator<std::input_iterator_tag, ValueType>
{
    friend class BaseTable<KeyType, ValueType>;
    friend class SimpleTable<KeyType, ValueType>;
    friend class SortTable<KeyType, ValueType>;
private:
    Iterator() {}
    Iterator(std::pair<KeyType, ValueType>* ptr) : p(ptr) {}
public:
    std::pair<KeyType, ValueType>* getPtr()
    {
        return p;
    }

    bool operator==(Iterator const& other) const
    {
        return p->first == other.p->first;
    }
    bool operator!=(Iterator const& other) const
    {
        return p->first != other.p->first;
    }
    typename Iterator::reference operator*() const
    {
        return p->second;
    }
    virtual Iterator& operator++()
    {
        ++p;
        return *this;
    }
    virtual Iterator& operator+(int index)
    {
        p += index;
        return *this;
    }
private:
    std::pair<KeyType, ValueType>* p = nullptr;
};

template <typename KeyType, typename ValueType>
struct Node {
public:
    KeyType key;
    ValueType value;
    Node* left;
    Node* right;
    Node* parent;

    Node(KeyType key, ValueType value) : key{ key }, value{ value }, left{ nullptr }, right{ nullptr }, parent { nullptr } {}

};

template <typename KeyType, typename ValueType>
class BinaryTreeIterator {
private:
    Node<KeyType, ValueType>* current;

    Node<KeyType, ValueType>* belowNode(Node<KeyType, ValueType>* node) const {
        if (node->right != nullptr) {
            return minHelper(node->right);
        }

        Node<KeyType, ValueType>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }
public:
    BinaryTreeIterator(Node<KeyType, ValueType>* node) { current = node; }

    ValueType& operator*() { return current->value; }
    Node<KeyType, ValueType>* operator->() { return current; }

    BinaryTreeIterator& operator++() {
        current = belowNode(current);
        return *this;
    }
    Node<KeyType, ValueType>* minHelper(Node<KeyType, ValueType>* node) const {
        if (node->left == nullptr) {
            return node;
        }
        else {
            return minHelper(node->left);
        }
    }
    bool operator==(const BinaryTreeIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const BinaryTreeIterator& other) const {
        return current != other.current;
    }
};

template <typename KeyType, typename ValueType>
struct NodeAVL {
public:
    KeyType key;
    ValueType value;
    NodeAVL* left;
    NodeAVL* right;
    NodeAVL* parent;
    int height; // новое поле для AVL дерева

    NodeAVL(KeyType key, ValueType value) : key{ key }, value{ value }, left{ nullptr }, right{ nullptr }, parent{ nullptr }, height{ 1 } {}
};

template <typename KeyType, typename ValueType>
class AVLTreeIterator {
private:
    NodeAVL<KeyType, ValueType>* current;

    NodeAVL<KeyType, ValueType>* belowNode(NodeAVL<KeyType, ValueType>* node) const {
        if (node->right != nullptr) {
            return minHelper(node->right);
        }

        NodeAVL<KeyType, ValueType>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }
public:
    AVLTreeIterator(NodeAVL<KeyType, ValueType>* node) { current = node; }

    ValueType& operator*() { return current->value; }
    NodeAVL<KeyType, ValueType>* operator->() { return current; }

    AVLTreeIterator& operator++() {
        current = belowNode(current);
        return *this;
    }
    NodeAVL<KeyType, ValueType>* minHelper(NodeAVL<KeyType, ValueType>* node) const {
        if (node->left == nullptr) {
            return node;
        }
        else {
            return minHelper(node->left);
        }
    }
    bool operator==(const AVLTreeIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const AVLTreeIterator& other) const {
        return current != other.current;
    }
};