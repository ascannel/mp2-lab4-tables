#include "gtest/gtest.h"
#include <table.hpp>
#include <random>


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
	HashTable<int, int> _table(9);
	_table.insert(3, 3);
	_table = table;
	EXPECT_EQ(1, _table.find(1)->second);
}

TEST(HashTable, can_balance) {
	HashTable<int, int> table(10);
	for (int i = 0; i < 100; i++) {
		table.insert(i, i);
	}
}

/*TEST(HashTable, can_balance_big_table) {
	HashTable<int, int> ht(10);
	for (int i = 0; i < 1000; i++) {
		ht.insert(i, i);
	}
}*/

TEST(HashTable, can_solve_collisions) {
	HashTable<int, int> table(10);
	for (int i = 0; i < 100; i+=10) {
		table.insert(i, i);
	}
}

TEST(BinaryTree, can_insert){
	BinaryTree<int, int> tree;
	tree.insert(2,3);
	EXPECT_EQ(tree->value, 3);

}

TEST(BinaryTree, can_insert_right){
	BinaryTree<int, int> tree;
	tree.insert(4,1);
	tree.insert(5,2);
	tree.insert(2,3);
	tree.insert(1,4);
	tree.insert(3,5);
	EXPECT_EQ(tree->value, 1);
	EXPECT_EQ(tree->right->value, 2);
	EXPECT_EQ(tree->left->value, 3);
	EXPECT_EQ(tree->left->left->value, 4);
	EXPECT_EQ(tree->left->right->value, 5);
}

TEST(BinaryTree, can_remove){
	BinaryTree<int,int> tree;
	tree.insert(5,1);
	tree.insert(4,2);
	tree.insert(3,3);
	tree.remove(5);
	ASSERT_EQ(tree->value, 2);
}

TEST(BinaryTree, can_remove_right){
	BinaryTree<int, int> tree;
	tree.insert(4,1);
	tree.insert(5,2);
	tree.insert(3,3);
	tree.insert(1,4);
	tree.remove(3);
	EXPECT_EQ(tree->left->value, 4);
}

TEST(BinaryTree, parents) {
	BinaryTree<int, int> tree;
	tree.insert(4, 1);
	tree.insert(5, 2);
	tree.insert(3, 3);
	EXPECT_EQ(tree.end()->parent->key, tree.begin()->parent->key);
}

TEST(BinaryTree, can_find){
	BinaryTree<int, int> tree;
	tree.insert(4,1);
	tree.insert(5,2);
	tree.insert(3,3);
	tree.insert(1,4);
	EXPECT_EQ(tree.find(1)->value, 4);
}

TEST(BinaryTree, iterator_works_with_changes_in_values) {
	BinaryTree<int, int> tree;
	for (int i = 0; i < 10; i++) {
		tree.insert(i, 1);
	}
	for (auto it = tree.begin();; it.operator++()) {
		it->value = it->value + 1;
		if (it == tree.end()) {
			break;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (tree.find(i)->value != 2) std::cout << i;
		EXPECT_EQ(tree.find(i)->value, 2);
	}
}

TEST(BinaryTree, can_insert_random_values) {
	BinaryTree<int, int> tree;
	std::vector<int> randNums;
	for (int i = 0; i < 100; i++) {
		randNums.push_back(i);
	}


	for (int i = 0; i < 100; i++) {
		int ind = rand() % randNums.size();
		tree.insert(randNums[ind],1);
		std::swap(randNums[ind], randNums[randNums.size() - 1]);
		randNums.pop_back();
	}
	for (int i = 0; i < 100; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
}

TEST(AVLTreeTest, can_insert_and_find) {
    AVLTree<int, int> avl;
    avl.insert(10, 100);
    avl.insert(20, 200);
    avl.insert(30, 300);

    EXPECT_EQ(avl.sizeTree(), 3);
    
    AVLTreeIterator<int, int> it = avl.find(20);
    EXPECT_EQ(it->key, 20);
    EXPECT_EQ(it->value, 200);

    EXPECT_THROW(avl.find(40), const char*);
}

TEST(AVLTreeTest, can_it_remove) {
    AVLTree<int, int> avl;
    avl.insert(10, 100);
    avl.insert(20, 200);
    avl.insert(30, 300);

    EXPECT_EQ(avl.sizeTree(), 3);

    avl.remove(20);
    EXPECT_EQ(avl.sizeTree(), 2);

    EXPECT_THROW(avl.find(20), const char*);
}

TEST(AVLTreeTest, can_it_get_begin_and_end) {
    AVLTree<int, int> avl;
    avl.insert(10, 100);
    avl.insert(20, 200);
    avl.insert(30, 300);

    AVLTreeIterator<int, int> it_begin = avl.begin();
    EXPECT_EQ(it_begin->key, 10);
    EXPECT_EQ(it_begin->value, 100);

    AVLTreeIterator<int, int> it_end = avl.end();
    EXPECT_EQ(it_end->key, 30);
    EXPECT_EQ(it_end->value, 300);
}

TEST(AVLTreeTest, can_it_get_height_and_balance) {
    AVLTree<int, int> avl;
    avl.insert(10, 100);
    avl.insert(20, 200);
    avl.insert(30, 300);
    avl.insert(5, 50);
    avl.insert(25, 250);
    avl.insert(35, 350);
    
    EXPECT_EQ(avl.getHeight(avl->left), 1);
    EXPECT_EQ(avl.getHeight(avl->right), 2);
    
    EXPECT_EQ(avl.getBalance(avl->left), -1);
    EXPECT_EQ(avl.getBalance(avl->right), 1);
}

TEST(AVLTreeTest, can_bigLeftRotate) {
    AVLTree<int, int> avl;
    avl.insert(10, 100);
    avl.insert(20, 200);
    avl.insert(30, 300);
    
    EXPECT_EQ(avl.getHeight(avl->right), 1);
    
    avl.insert(25, 250);
    avl.insert(35, 350);

    EXPECT_EQ(avl.getHeight(avl->right), 2);
    EXPECT_EQ(avl.getHeight(avl->right->left), 1);
    EXPECT_EQ(avl.getHeight(avl->right->right), 1);

    avl.remove(20);
    EXPECT_EQ(avl.getHeight(avl->right), 1);
    EXPECT_EQ(avl.getHeight(avl->right->left), 1);
    EXPECT_EQ(avl.getHeight(avl->right->right), 1);
}

TEST(AVLTreeTest, can_bigRightRotate) {
    AVLTree<int, int> avl;
    avl.insert(30, 300);
    avl.insert(20, 200);
    avl.insert(10, 100);
    
    EXPECT_EQ(avl.getHeight(avl->left), 1);
    
    avl.insert(25, 250);
    avl.insert(5, 50);

    EXPECT_EQ(avl.getHeight(avl->left), 2);
    EXPECT_EQ(avl.getHeight(avl->left->left), 1);
    EXPECT_EQ(avl.getHeight(avl->left->right), 1);

    avl.remove(20);
    EXPECT_EQ(avl.getHeight(avl->left), 1);
    EXPECT_EQ(avl.getHeight(avl->left->left), 1);
    EXPECT_EQ(avl.getHeight(avl->left->right), 1);
}

TEST(AVLTree, can_insert) {
	AVLTree<int, std::string> tree;
	tree.insert(5, "five");
	tree.insert(3, "three");
	tree.insert(7, "seven");
	EXPECT_EQ(tree.sizeTree(), 3);
}

TEST(AVLTree, can_remove) {
	AVLTree<int, int> tree;
	tree.insert(5, 5);
	tree.insert(3, 3);
	tree.insert(7, 7);
	tree.remove(7);
	EXPECT_EQ(tree.sizeTree(), 2);
}

TEST(AVLTree, can_find) {
	AVLTree<int, std::string> tree;
	tree.insert(5, "five");
	tree.insert(3, "three");
	tree.insert(7, "seven");
	AVLTreeIterator<int, std::string> it = tree.find(3);
	EXPECT_EQ((*it), "three");
}

TEST(AVLTree, iterator_works) {
	AVLTree<int, std::string> tree;
	tree.insert(5, "five");
	tree.insert(3, "three");
	tree.insert(7, "seven");

	AVLTreeIterator<int, std::string> it = tree.begin();
	EXPECT_EQ((*it), "three");

	++it;
	EXPECT_EQ((*it), "five");

	++it;
	EXPECT_EQ((*it), "seven");
}

TEST(AVLTree, iterator_works_with_changes_in_values) {
	AVLTree<int, int> tree;
	tree.insert(5, 5);
	tree.insert(3, 3);
	tree.insert(7, 7);

	AVLTreeIterator<int, int> it = tree.begin();
	it->value += 1;
	EXPECT_EQ((*it), 4);
	++it;
	it->value += 1;
	EXPECT_EQ((*it), 6);
	++it;
	it->value += 1;
	EXPECT_EQ((*it), 8);
}