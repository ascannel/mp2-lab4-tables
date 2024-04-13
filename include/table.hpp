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

template <typename KeyType, typename ValueType>
class BinaryTree {
private:
    Node<KeyType, ValueType>* root;
    int size;
public:
    BinaryTree() : root{ nullptr }, size{ 0 } {}

    ~BinaryTree() {}

    void insert(const KeyType& key, ValueType value) {
        root = insertNode(root, key, value);
    }

    void remove(const KeyType& key, ValueType value) {
        root = removeNode(root, key, value);
    }

    void inorder() {
        inorderNode(root);
    }

    BinaryTreeIterator<KeyType, ValueType>* find(const KeyType& key) const {
        Node<KeyType, ValueType>* node = findNode(key);
        return BinaryTreeIterator<KeyType, ValueType>(node, root, size);
    }

    BinaryTreeIterator<KeyType, ValueType> begin() const {
        Node<KeyType, ValueType>* node = root;
        while (node->left != nullptr) {
            node = node->left;
        }
        return BinaryTreeIterator<KeyType, ValueType>(node, root, size);
    }

    BinaryTreeIterator<KeyType, ValueType> end() const {
        Node<KeyType, ValueType>* node = root;
        while (node->right != nullptr) {
            node = node->right;
        }
        return BinaryTreeIterator<KeyType, ValueType>(node, root, size);
    }

    Node<KeyType, ValueType>* minValueNode(Node<KeyType, ValueType>* node) {
        Node<KeyType, ValueType>* current = node;
        while (current && current->left != NULL) {
            current = current->left;
        }
        return current;
    }

    Node<KeyType, ValueType>* insertNode(Node<KeyType, ValueType>* node, KeyType key, ValueType value) {
        if (node == nullptr) {
            return new Node<KeyType, ValueType>(key, value);
        }
        if (value <= node->value) {
            node->left = insertNode(node->left, key, value);
        }
        else {
            node->right = insertNode(node->right, key, value);
        }
        size++;
        return node;
    }

    Node<KeyType, ValueType>* removeNode(Node<KeyType, ValueType>* node, KeyType key, ValueType value) {
        if (node == NULL) {
            return node;
        }
        if (value < node->value) {
            node->left = removeNode(node->left, key, value);
        }
        else if (value > node->value) {
            node->right = removeNode(node->right, key, value);
        }
        else {
            if (node->left == nullptr) {
                Node<KeyType, ValueType>* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node<KeyType, ValueType>* temp = node->left;
                delete node;
                return temp;
            }
            Node<KeyType, ValueType>* temp = minValueNode(node->right);
            node->value = temp->key;
            node->right = removeNode(node->right, temp->key, temp->value);
        }
        return node;
    }

    Node<KeyType, ValueType>* findNode(KeyType& key) {
        Node<KeyType, ValueType>* node = root;
        while (node->key != key) {
            if (key <= node->key && node->left != nullptr) {
                node = node->left;
            }
            else if (key > node->key && node->right != nullptr) {
                node = node->right;
            }
            else {
                throw "Invalid value";
            }
        }
        return node;
    }
    void inorderNode(Node<KeyType, ValueType>* node) {
        if (node != nullptr) {
            inorderNode(node->left);
            inorderNode(node->right);
        }
    }
};

