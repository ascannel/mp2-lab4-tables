#pragma once
#include <vector>

template<typename KeyType, typename ValueType>
class BaseTable;

template<typename KeyType, typename ValueType>
class SimpleTable;

template<typename KeyType, typename ValueType>
class Iterator : public std::iterator<std::input_iterator_tag, ValueType>
{
    friend class BaseTable<KeyType, ValueType>;
    friend class SimpleTable<KeyType, ValueType>;
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