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
    HashTable<int, int> table(100);
}

TEST(HashTable, can_insert_hash_table) {
	HashTable<float, int> table(10);
	int res1 = table.insert(3.14, 1)->second;
	int et1 = 1;
	EXPECT_EQ(et1, res1);
}

TEST(HashTable, can_insert_to_empty_hash_table) {
	HashTable<int, int> table(0);
	for (int i = 0; i < 10; i++) {
		table.insert(i, 1);
	}
	for (int i = 0; i < 10; i++) {
		EXPECT_EQ(table[i], 1);
	}
}

// TODO: add int size()
// TODO: add stress test for 1000+ elements

TEST(HashTable, can_insert) {
	HashTable<long long, int> table(5);
	table.insert(4.8, 10);
	table.insert(4.9, 20);
	EXPECT_EQ(table.find(4.8)->second, 10);
}

TEST(HashTable, can_find) {
	HashTable<float, int> table(5);
	table.insert(4.8, 10);
	table.insert(4.9, 20);
	EXPECT_EQ(table.find(4.8)->second, 10);
}

TEST(HashTable, can_remove) {
	HashTable<std::string, int> table(10);
	table.insert("gold", 1);
	table.insert("silver", 2);
	table.insert("platinum", 3);

	EXPECT_TRUE(table.remove("gold"));
	EXPECT_EQ(table["silver"], 2);
	EXPECT_EQ(table["platinum"], 3);
}

TEST(HashTable, iterator_works) {
	HashTable<double, int> ht(5);
	ht.insert(3.17, 10);
	ht.insert(6.08, 20);
	ht.insert(5.01, 30);
	int sum = 0;
	for (auto it = ht.begin(); it != ht.end(); ++it)
	{
		sum += it->second;
	}
	EXPECT_EQ(sum, 60);
}

TEST(HashTable, iterator_works_with_changes_in_values) {
	HashTable<int, int> table(10);
	for (int i = 0; i < 10; i++) {
		table.insert(i, 1);
	}
	for (auto it = table.begin(); it != table.end(); it.operator++()) {
		it->second++;
	}
	for (int i = 0; i < 10; i++) {
		EXPECT_EQ(table[i], 2);
	}
}

TEST(HashTable, can_equal_assign) {
	HashTable<int, int> table(10);
	table.insert(1, 1);
	table.insert(2, 2);
	table = table;
	EXPECT_EQ(1, table.find(1)->second);
	EXPECT_EQ(2, table.find(2)->second);
}

TEST(HashTable, can_non_equal_assign) {
	HashTable<int, int> table(10);
	table.insert(1, 1);
	table.insert(2, 2);
	HashTable<int, int> atable(9);
	atable.insert(3, 3);
	atable = table;
	EXPECT_EQ(1, atable.find(1)->second);
}

TEST(HashTable, can_balance) {
	HashTable<int, int> table(10);
	for (int i = 0; i < 100; i++) {
		table.insert(i, i);
	}
	std::cout << table;
}

TEST(HashTable, can_solve_collisions) {
	HashTable<int, int> table(10);
	for (int i = 0; i < 100; i+=10) {
		table.insert(i, i);
	}
	std::cout << table;
}
// HashTable
// TODO: add int size()
// TODO: add stress test for 1000+ elements

//BinaryTree Tests:
// insert, remove, find, iterator with changes in values
