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
