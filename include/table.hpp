#pragma once
#include "iterator.hpp"

template<typename KeyType, typename ValueType>
class BaseTable
{
public:
    BaseTable() {}
    virtual Iterator<KeyType, ValueType> begin()
    {
        return Iterator<KeyType, ValueType>();
    }
    virtual Iterator<KeyType, ValueType> end()
    {
        return Iterator<KeyType, ValueType>();
    }
    virtual Iterator<KeyType, ValueType> find(const KeyType& key) = 0;
    virtual Iterator<KeyType, ValueType> insert(const KeyType& key, const ValueType& value) = 0;
    virtual void remove(const KeyType& key) = 0;
    virtual void remove(Iterator<KeyType, ValueType>& it) {}

    virtual ValueType& operator[](const KeyType& key) = 0;
    virtual size_t getSize()
    {
        return end().getPtr() - begin().getPtr();
    }

    virtual Iterator<KeyType, ValueType> getMin() { return end(); }
    virtual Iterator<KeyType, ValueType> getMax() { return end(); }
};

template<typename KeyType, typename ValueType>
class SimpleTable : public BaseTable<KeyType, ValueType>
{
    std::vector<std::pair<KeyType, ValueType> > keyData;
public:
    virtual Iterator<KeyType, ValueType> begin() override
    {
        if (keyData.size() == 0ull)
            return Iterator<KeyType, ValueType>(nullptr);
        return &(keyData.front());
    }
    virtual Iterator<KeyType, ValueType> end() override
    {
        if (keyData.size() == 0ull)
            return Iterator<KeyType, ValueType>(nullptr);
        return &(keyData.back()) + 1ull;
    }
    Iterator<KeyType, ValueType> find(const KeyType& key) override
    {
        for (size_t i = 0; i < keyData.size(); i++)
        {
            if (keyData[i].first == key)
                return Iterator<KeyType, ValueType>(&keyData[i]);
        }
        return Iterator<KeyType, ValueType>(&keyData.back() + 1ull);
    }
    Iterator<KeyType, ValueType> insert(const KeyType& key, const ValueType& value) override
    {
        keyData.push_back(std::make_pair(key, value));
        return Iterator<KeyType, ValueType>(&keyData.back() - 1ull);
    }
    virtual void remove(const KeyType& key) override
    {
        Iterator<KeyType, ValueType> iter(find(key));
        if (iter != end())
        {
            keyData.erase(keyData.begin() + (iter.getPtr() - begin().getPtr()));
        }
    }
    virtual ValueType& operator[](const KeyType& key) override
    {
        return *(find(key));
    }
    Iterator<KeyType, ValueType> get_min_by_key()
    {
        if (keyData.size() == 0ull)
            return Iterator<KeyType, ValueType>(nullptr);
        auto iter = begin();
        auto min = iter;
        while (iter != end())
        {
            if (iter.getPtr()->first < min.getPtr()->first)
                min = iter;
            ++iter;
        }
        return min;
    }
    Iterator<KeyType, ValueType> get_max_by_key()
    {
        if (keyData.size() == 0ull)
            return Iterator<KeyType, ValueType>(nullptr);
        auto iter = begin();
        auto max = iter;
        while (iter != end())
        {
            if (iter.getPtr()->first > max.getPtr()->first)
                max = iter;
            ++iter;
        }
        return max;
    }
};

template<typename KeyType, typename ValueType>
class SortTable : public SimpleTable<KeyType, ValueType>
{
    std::vector<std::pair<KeyType, ValueType> > keyData;
public:
    Iterator<KeyType, ValueType> begin() override
    {
        if (keyData.size() == 0ull)
            return Iterator<KeyType, ValueType>();
        return &(keyData.front());
    }
    Iterator<KeyType, ValueType> end() override
    {
        if (keyData.size() == 0ull)
            return Iterator<KeyType, ValueType>();
        return &(keyData.back()) + 1ull;
    }
    Iterator<KeyType, ValueType> find(const KeyType& key) override
    {
        size_t left = 0, right = keyData.size();
        while (left + 1 < right) {
            int med = (left + right) / 2;
            if (keyData[med].first <= key)
                left = med;
            else
                right = med;
        }
        if (left < keyData.size() && keyData[left].first == key)
            return Iterator<KeyType, ValueType>(&keyData[0] + left);
        else
            return Iterator<KeyType, ValueType>(&keyData[0] + keyData.size());

    }
    Iterator<KeyType, ValueType> insert(const KeyType& key, const ValueType& value) override
    {
        size_t left = 0, right = keyData.size(), med = 0;
        while (left < right)
        {
            med = (right - left) / 2 + left;
            if (key < keyData[med].first)
                right = med;
            else
                left = med + 1;
        }

        auto pair = std::make_pair(key, value);
        if (left == keyData.size())
        {
            keyData.push_back(pair);
            return Iterator<KeyType, ValueType>(&keyData.back() - 1);
        }

        auto it = keyData.begin();
        std::advance(it, left);
        keyData.insert(it, pair);
        return Iterator<KeyType, ValueType>(&keyData[left] - 1);

    }
    virtual void remove(const KeyType& key) override
    {
        Iterator<KeyType, ValueType> it = find(key);
        keyData.erase(std::remove(keyData.begin(), keyData.end(), *it.getPtr()));
    }
    virtual ValueType& operator[](const KeyType& key) override
    {
        return *(find(key));
    }
};

template<typename KeyType, typename ValueType>
class HashTable
{
    std::hash<KeyType> hasher;
    std::vector<SortTable<KeyType, ValueType>> nodes;

    size_t getPos(KeyType const& key)
    {
        return  hasher(key) % nodes.size();
    }


public:
    template<class KeyType, class ValueType>
    class HashTableIterator : public std::iterator<std::input_iterator_tag, ValueType>
    {
        friend class HashTable<KeyType, ValueType>;

    public:

        HashTableIterator(std::vector<SortTable<KeyType, ValueType>>& nodes, size_t pos, Iterator<KeyType, ValueType> it)
            : nodes(nodes), pos(pos), itPair(it) { }

        std::pair<KeyType, ValueType>* getPtr()
        {
            return itPair->getPtr();
        }

        bool operator==(HashTableIterator const& other) const
        {
            return itPair == other.itPair;
        }

        bool operator!=(HashTableIterator const& other) const
        {
            return itPair != other.itPair;
        }

        typename HashTableIterator::reference operator*() const
        {
            return *itPair;
        }

        virtual HashTableIterator& operator++()
        {
            if (itPair == nodes[pos].end())
                return *this;

            Iterator<KeyType, ValueType> tmpIter = itPair;
            size_t tmpPos = pos;

            if (++itPair != nodes[pos].end())
                return *this;

            for (size_t i = tmpPos + 1; i < nodes.size(); i++)
                if (nodes[i].begin() != nodes[i].end())
                {
                    tmpPos = i;
                    tmpIter = nodes[i].begin();
                    break;
                }

            pos = tmpPos;
            itPair = tmpIter;
            return *this;
        }

        virtual HashTableIterator& operator+(int ind)
        {
            for (int i = 0; i < ind; i++)
                operator++();
            return *this;
        }

    private:
        std::vector<SortTable<KeyType, ValueType>>& nodes;
        size_t pos;
        Iterator<KeyType, ValueType> itPair;
    };

    HashTable(int num = 5) : nodes(num) { }
    HashTableIterator<KeyType, ValueType> begin()
    {
        for (size_t i = 0; i < nodes.end(); i++)
            if (nodes[i].begin() != nodes[i].end())
                return HashTableIterator<KeyType, ValueType>(nodes, i, nodes[i].begin());
        return HashTableIterator<KeyType, ValueType>(nodes, nodes.size() - 1, nodes.back().end());
    }
    HashTableIterator<KeyType, ValueType> end()
    {
        for (int i = nodes.size() - 1; i >= 0; i--)
            if (nodes[i].begin() != nodes[i].end())
                return HashTableIterator<KeyType, ValueType>(nodes, i, nodes[i].end());
        return HashTableIterator<KeyType, ValueType>(nodes, nodes.size() - 1, nodes.back().end());
    }
    HashTableIterator<KeyType, ValueType> find(const KeyType& key)
    {
        size_t pos = getPos(key);
        Iterator<KeyType, ValueType> itPair = nodes[pos].find(key);
        return HashTableIterator<KeyType, ValueType>(nodes, pos, itPair);
        return end();
    }

    HashTableIterator<KeyType, ValueType> insert(const KeyType& key, const ValueType& value)
    {
        size_t pos = getPos(key);
        Iterator<KeyType, ValueType> itPair = nodes[pos].insert(key, value);
        return HashTableIterator<KeyType, ValueType>(nodes, pos, itPair);
    }

    void remove(const KeyType& key)
    {
        size_t pos = getPos(key);
        nodes[pos].remove(key);
    }

    virtual ValueType& operator[](const KeyType& key)
    {
        return *(find(key));
    }

    void print()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            nodes[i].print();
        }
    }

};
template<typename KeyType, typename ValueType>
class AvlTable : public BaseTable<KeyType, ValueType>
{
        struct Node
        {
            std::pair<KeyType, ValueType> pair;
            Node* left;
            Node* right;
            Node(std::pair<KeyType, ValueType> _pair)
            {
                pair = _pair;
                left = nullptr;
                right = nullptr;
            }
            ~Node()
            {
                delete left;
                delete right;
            }
        };

        Node* root;
        size_t size = 0;

        Node* insert(Node* node, const KeyType& key, const ValueType& value)
        {
            if (!node)
            {
                return new Node(std::make_pair(key, value));
            }
            else
            {
                if (key < node->pair.first)
                {
                    node->left = insert(node->left, key, value);
                }
                else
                {
                    node->right = insert(node->right, key, value);
                }
            }
            return node;
        }

        Node* find(Node* node, const KeyType& key)
        {
            Node* ans;

            if (!node)
            {
                return nullptr;
            }

            if (key == node->pair.first)
            {
                return node;
            }

            if (key < node->pair.first)
            {
                ans = find(node->left, key);
            }
            else
            {
                ans = find(node->right, key);
            }
            return ans;
        }

        Node* remove(Node* node, const KeyType& key)
        {
            if (!node)
            {
                return nullptr;
            }
            if (key < node->pair.first)
            {
                node->left = remove(node->left, key);
                return node;
            }
            else if (key > node->pair.first)
            {
                node->right = remove(node->right, key);
                return node;
            }
            else
            {
                if (!node->left && !node->right)
                {
                    return nullptr;
                }
                if (!node->left)
                {
                    return node->right;
                }
                if (!node->right)
                {
                    return node->left;
                }
                Node* minRight = node->right;
                while (minRight->left)
                {
                    minRight = minRight->left;
                }
                node->pair = minRight->pair;
                node->right = remove(node->right, node->pair.first);
                return node;
            }
        }

        void printLeft(Node* node)
        {
            if (node)
            {
                printLeft(node->left);
                std::cout << node->pair.first << " " << node->pair.second << std::endl;
                printLeft(node->right);
            }
        }
public:
        AvlTable()
        {
            root = nullptr;
            size = 0;
        }

        virtual size_t getSize() override
        {
            return size;
        }

        virtual Iterator<KeyType, ValueType> find(const KeyType& key) override
        {
            auto ans = find(root, key);
            if (ans == nullptr)
            {
                return this->end();
            }
            return Iterator<KeyType, ValueType>(&ans->pair);
        }

        virtual Iterator<KeyType, ValueType> insert(const KeyType& key, const ValueType& value)
        {
            root = insert(root, key, value);
            auto ans = std::make_pair(key, value);
            size++;
            return Iterator<KeyType, ValueType>(&ans);
        }

        virtual void remove(const KeyType& key) override
        {
            root = remove(root, key);
        }

        virtual ValueType& operator[](const KeyType& key) override
        {
            auto ans = find(root, key);
            return ans->pair.second;
        }

        void print()
        {
            printLeft(root);
        }

        virtual Iterator<KeyType, ValueType> getMax() override
        {
            if (!root)
            {
                return this->begin();
            }
            Node* current = root;
            while (current->right)
            {
                current = current->right;
            }
            return Iterator<KeyType, ValueType>(&current->pair);
        }

        virtual Iterator<KeyType, ValueType> getMin() override
        {
            if (!root)
            {
                return this->begin();
            }
            Node* current = root;
            while (current->left)
            {
                current = current->left;
            }
            return Iterator<KeyType, ValueType>(&current->pair);
        }
};