#pragma once
#include "iterator.hpp"
#include <iostream>

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

template <typename KeyType, typename ValueType>
class HashTable {
private:
    std::vector<std::pair<KeyType, ValueType>>* array;
    int length;
public:
    class HashIterator
    {
    private:
        std::vector<std::pair<KeyType, ValueType>>* array_;
        size_t counter_;
        size_t number_;
        size_t length_;
    public:
        HashIterator(std::vector<std::pair<KeyType, ValueType>>* array, size_t counter, size_t number, size_t length) : array_(array), counter_(counter), number_(number), length_(length) {}
        HashIterator& operator++()
        {
            if (array_[counter_].size() == number_ + 1)
            {
                counter_++;
                while (array_[counter_].size() == 0)
                {
                    if (counter_ == length_)
                        return *this;
                    number_ = 0;
                    counter_++;
                }
            }
            else
            {
                number_++;
            }
            return *this;
        }
        static HashIterator begin(std::vector<std::pair<KeyType, ValueType>>* array, size_t length)
        {
            for (size_t i = 0; i < length; i++)
            {
                if (array[i].size() != 0)
                    return HashIterator(array, i, 0, length);
            }
            return HashIterator(array, length, 0, length);
        }
        std::pair<KeyType, ValueType>& operator*()
        {
            return array_[counter_][number_];
        }
        bool operator ==(const HashIterator& other)
        {
            return (array_ == other.array_ && counter_ == other.counter_ && number_ == other.number_);
        }
        bool operator !=(const HashIterator& other)
        {
            return !(*this == other);
        }
        std::pair<KeyType, ValueType>* operator->()
        {
            return &**this;
        }
    };
    HashTable(int ptr) {
        array = new std::vector<std::pair<KeyType, ValueType>>[ptr];
        length = ptr;
    }
    HashTable(const HashTable& table) {
        length = table.length;
        array = new std::vector<std::pair<KeyType, ValueType>>[length];
        for (int i = 0; i < length; i++) {
            array[i] = table.array[i];
        }
    }
    HashTable& operator=(const HashTable& table) {
        if (this == &table) {
            return *this;
        }
        else {
            length = table.length;
            array = new std::vector<std::pair<KeyType, ValueType>>[table.length];
            for (int i = 0; i < table.length; i++) {
                array[i] = table.array[i];
            }
        }
        return *this;
    }
    ~HashTable() {
        length = 0;
        delete[] array;
    }
    int hash(std::string s) {
        char al[] = "abcdefghijklmnopqrstuvwxyz";
        int sum = 0;
        for (int i = 0; i < s.length(); i++) {
            char c = s[i];
            for (int j = 0; j < 26; j++) {
                if (c == al[j]) {
                    sum += pow(j, i);
                }
            }
        }
        return sum % length;
    }
    int hash(char c) {
        char al[] = "abcdefghijklmnopqrstuvwxyz";
        int sum = 0;
        for (int j = 0; j < 26; j++) {
            if (c == al[j]) {
                sum += pow(j, i);
            }
        }
        return sum % length;
    }
    int hash(long long i) {
        return i % length;
    }
    int hash(int i) {
        return i % length;
    }
    int hash(double i) {
        i = abs(i);
        int ptr;
        int man = frexp(i, &ptr);
        int ord = trunc(i);
        int res = man + pow(2, ord);
        return res % length;
    }
    int hash(float i) {
        i = abs(i);
        int ptr;
        int man = frexp(i, &ptr);
        int ord = trunc(i);
        int res = man + pow(2, ord);
        return res % length;
    }
    int hash(std::vector<int> v) {
        int sum = 0;
        for (int i = 0; i < v.size(); i++) {
            sum += v[i];
        }
        return sum % length;
    }
    auto insert(KeyType key, const ValueType& data) {
        int pos = hash(key);
        array[pos].push_back(std::make_pair(key, data));
        int cap = checkCollisions();
        if (cap) {
            *this = this->balanceCollisions(cap);
        }
        pos = hash(key);
        for (auto it = array[pos].begin(); it != array[pos].end(); it++) {
            if (it->first == key) {
                return it;
            }
        }

    }
    HashIterator find(const KeyType& key) {
        int pos = hash(key);
        for (auto it = array[pos].begin(); it != array[pos].end(); it++) {
            if (it->first == key) {
                return HashIterator(array, pos, it - array[pos].begin(), length);
            }
        }
        return HashIterator(array, length, 0, length);
    }
    bool remove(const KeyType& key) {
        int pos = hash(key);
        for (auto it = array[pos].begin(); it != array[pos].end(); it++) {
            if (it->first == key) {
                array[pos].erase(it);
                return true;
            }
        }
        return false;
    }
    ValueType& operator[](const KeyType& key) {
        int pos = hash(key);
        for (auto it = array[pos].begin(); it != array[pos].end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }
        throw std::runtime_error("Invalid key!");
    }

    HashIterator begin() {
        return HashIterator::begin(array, length);
    }
    HashIterator end() {
        return HashIterator(array, length, 0, length);
    }
    int checkCollisions() {
        int sum = 0;
        for (int i = 0; i < length; i++) {
            sum += array[i].size();
        }
        if (sum > length) {
            return sum;
        }
        else {
            return 0;
        }
    }
    HashTable balanceCollisions(int newLength) {
        HashTable<KeyType, ValueType> table(newLength);
        for (int i = 0; i < this->length; i++) {
            for (auto it = this->array[i].begin(); it != this->array[i].end(); it++) {
                table.insert(it->first, it->second);
            }
        }
        return table;
    }
    friend std::ostream& operator<<(std::ostream& out, const HashTable& table) {
        for (int i = 0; i < table.length; i++) {
            for (auto it = table.array[i].begin(); it != table.array[i].end(); it++) {
                std::cout <<"string: " << i << " " << "key: " << it->first << " " << "value: " << it->second << std::endl;
            }
        }
        return out;
    }
};


template <typename T>
class Node {
public:
    T key;
    Node* left;
    Node* right;
    int height;

    Node(const T& key) : key{ key }, left{ nullptr }, right{ nullptr }, height{ 1 } {}
};

template <typename T>
class BinaryTreeIterator {
public:
    BinaryTreeIterator(Node<T>* node) : current{ node } {}

    T& operator*() { 
        return current->key; 
    }
    T* operator->() { 
        return &(current->key); 
    }

    BinaryTreeIterator<T>& operator++() {
        current = successorNode(current);
        return *this;
    }
    Node<T>* minNode(Node<T>* node) const {
        if (node->left == nullptr) {
            return node;
        }
        else {
            return minNode(node->left);
        }
    }
    bool operator==(const BinaryTreeIterator<T>& other) const {
        return current == other.current;
    }

    bool operator!=(const BinaryTreeIterator<T>& other) const {
        return current != other.current;
    }

private:
    Node<T>* current;
    Node<T>* successorNode(Node<T>* node) const {
        if (node->right != nullptr) {
            return minNode(node->right);
        }

        Node<T>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }
};
template <typename T>
class BinaryTree {
private:
    Node<T>* root;
    int size;

    BinaryTree() : root{ nullptr }, size{ 0 } {}

    ~BinaryTree() {}

    bool insert(const T& key) {
        root = insertNode(root, key);
        if (root != nullptr) {
            size++;
            return true;
        }
        return false;
    }

    bool remove(const T& key) {
        if (removeNode(root, key)) {
            size--;
            return true;
        }
        return false;
    }

    bool find(const T& key) const {
        return findNode(root, key);
    }

    T min() const {
        return minNode(root)->key;
    }

    T max() const {
        return maxNode(root)->key;
    }

    BinaryTreeIterator<T> begin() const {
        return BinaryTreeIterator<T>(minNode(root));
    }

    BinaryTreeIterator<T> end() const {
        return BinaryTreeIterator<T>(nullptr);
    }

    Node<T>* insertNode(Node<T>* node, const T& key) {
        if (node == nullptr) {
            return new Node<T>(key);
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key);
        }
        else if (key > node->key) {
            node->right = insertNode(node->right, key);
        }

        updateHeight(node);
        return rebalance(node);
    }

    bool removeNode(Node<T>*& node, const T& key) {
        if (node == nullptr) {
            return false;
        }

        if (key < node->key) {
            return removeNode(node->left, key);
        }
        else if (key > node->key) {
            return removeNode(node->right, key);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
                return true;
            }
            else if (node->left == nullptr) {
                Node<T>* temp = node;
                node = node->right;
                delete temp;
                return true;
            }
            else if (node->right == nullptr) {
                Node<T>* temp = node;
                node = node->left;
                delete temp;
                return true;
            }
            else {
                Node<T>* successor = minNode(node->right);
                node->key = successor->key;
                return removeNode(node->right, successor->key);
            }
        }
    }

    bool findNode(Node<T>* node, const T& key) const {
        if (node == nullptr) {
            return false;
        }

        if (key < node->key) {
            return findNode(node->left, key);
        }
        else if (key > node->key) {
            return findNode(node->right, key);
        }
        else {
            return true;
        }
    }

    Node<T>* minNode(Node<T>* node) const {
        if (node->left == nullptr) {
            return node;
        }
        else {
            return minNode(node->left);
        }
    }

    Node<T>* maxNode(Node<T>* node) const {
        if (node->right == nullptr) {
            return node;
        }
        else {
            return maxNode(node->right);
        }
    }
};

