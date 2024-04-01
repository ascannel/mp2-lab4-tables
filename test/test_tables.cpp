#include <gtest.h>
#include <table.hpp>

TEST(SimpleTable, can_insert_items_in_table)
{
SimpleTable<int,int> table;
table.insert(1,5);
EXPECT_EQ(table[1],5);
}

TEST(SimpleTables, can_find_element)
{
SimpleTable<int,int> table,test_table;
table.insert(1,5);
EXPECT_EQ(table.find(1).getPtr()->second,5);
}

TEST(SimpleTables, can_get_begin)
{
SimpleTable<int,int> table;
table.insert(1,5);
table.insert(2,6);
auto it = table.find(1);
EXPECT_EQ(table.begin(),it);
}

TEST(SimpleTables, can_get_end)
{
SimpleTable<int,int> table;
table.insert(1,5);
table.insert(2,6);
auto it = table.begin()+2ull;
EXPECT_EQ(table.end(),it);
}

TEST(SimpleTable, can_remove_items_in_table) {
SimpleTable<int, int> table;
table.insert(2, 5);
table.insert(1, 6);
table.remove(2);
EXPECT_EQ(table.begin().getPtr()->second,6);
}

TEST(SortTable, can_it_sort)
{
    SortTable<int,int>table;
    table.insert(7,5);
    table.insert(5,6);
    table.insert(1,8);
    EXPECT_EQ(table.begin().getPtr()->second,8);
    EXPECT_EQ((table.begin()+2ull).getPtr()->second,5);
}

TEST(SortTable, can_get_size_of_table)
{
SortTable<int,int>table;
table.insert(1,5);
table.insert(2,6);
EXPECT_EQ(table.getSize(),2);
}

TEST(SortTable, can_insert_items_in_table1)
{
    SortTable<int,int>table;
    table.insert(1,5);
    EXPECT_EQ(table[1],5);
}

TEST(SortTables, can_find_element)
{
    SortTable<int,int>table,test_table;
    table.insert(1,5);
    EXPECT_EQ(table.find(1).getPtr()->second,5);
}

TEST(SortTables, can_get_begin)
{
    SortTable<int,int>table;
    table.insert(3,5);
    table.insert(2,6);
    auto it = table.find(2);
    EXPECT_EQ(table.begin(),it);
}

TEST(SortTables, can_get_end)
{
    SortTable<int,int>table;
    table.insert(1,5);
    table.insert(2,6);
    auto it = table.begin()+2ull;
    EXPECT_EQ(table.end(),it);
}

TEST(SortTable, can_remove_items_in_table) {
    SortTable<int, int> table;
    table.insert(2, 5);
    table.insert(1, 6);
    table.remove(1);
    EXPECT_EQ(table.begin().getPtr()->second,5);
}

TEST(SortTable, can_find_in_odd_table) {
    SortTable<int, int> table;
    table.insert(0, 1);
    table.insert(1, 2);
    table.insert(2, 3);
    EXPECT_EQ(table.find(1).getPtr()->second, 2);
}

TEST(SortTable, can_find_in_even_table) {
    SortTable<int, int> table;
    table.insert(0, 1);
    table.insert(1, 2);
    table.insert(2, 3);
    table.insert(3, 4);
    EXPECT_EQ(table.find(2).getPtr()->second, 3);
}

TEST(HashTable, can_create_hash_table) {
    HashTable<int, int> table;
}