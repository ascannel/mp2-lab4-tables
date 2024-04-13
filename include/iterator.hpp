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

    Node(KeyType key, ValueType value) : key{ key }, value{ value }, left{ nullptr }, right{ nullptr } {}
};

template <typename KeyType, typename ValueType>
class BinaryTreeIterator {
private:
    Node<KeyType, ValueType>* node;
    std::vector<Node<KeyType, ValueType>> order;
    std::vector<bool> was;
public:
    BinaryTreeIterator(Node<KeyType, ValueType>* _node, Node<KeyType, ValueType>* root, int size) {
        node = _node;
        was = vector<bool>(size);
        DFS(*root);
        sort(order);
    }

    void DFS(Node<KeyType, ValueType>& node) {
        if (node.left != NULL && !was[node.left->value]) {
            DFS(*node.left);
        }
        was[node.value] = true;
        if (node.right != NULL && !was[node.right->value]) {
            DFS(*node.right);
        }
        order.push_back(node);
    }
    void sort(std::vector<Node<KeyType, ValueType>>& order) {
        for (int i = 0; i < order.size(); i++) {
            for (int j = 0; j < order.size(); j++) {
                if (order[i].value < order[j].value) {
                    std::swap(order[i], order[j]);
                }
            }
        }
    }

    ValueType& operator*() {
        return node->value;
    }
    Node<KeyType, ValueType>* operator->() {
        return this->node;
    }

    BinaryTreeIterator& operator++() {
        for (int i = 0; i < order.size(); i++) {
            if (order[i].key == node->key) {
                this->node = &order[i + 1];
                return *this;
            }
        }
    }
    Node<KeyType, ValueType>* minNode(Node<KeyType, ValueType>* node) const {
        Node<KeyType, ValueType>* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    bool operator==(const BinaryTreeIterator<KeyType, ValueType>& other) const {
        return (node == other.node);
    }

    bool operator!=(const BinaryTreeIterator<KeyType, ValueType>& other) const {
        return (node != other.node);
    }
};