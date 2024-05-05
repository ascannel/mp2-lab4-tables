#include "gtest.h"
#include <table.hpp>
#include <random>
#include <chrono>
#include <numeric>

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
	tree.insert(3, 4);
	tree.insert(1, 2);
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
	for (int i = 0; i < 1000; i++) {
		randNums.push_back(i);
	}


	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randNums.size();
		tree.insert(randNums[ind],1);
		std::swap(randNums[ind], randNums[randNums.size() - 1]);
		randNums.pop_back();
	}
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
}

TEST(AVLTree, can_it_get_height_and_balance) {
    AVLTree<int, int> avl;
    avl.insert(10, 100);
    avl.insert(20, 200);
    avl.insert(30, 300);
    avl.insert(5, 50);
    avl.insert(25, 250);
    avl.insert(35, 350);
    EXPECT_EQ(avl.getHeight(avl->left), 2);
    EXPECT_EQ(avl.getHeight(avl->right), 2);
}

TEST(AVLTree, can_bigLeftRotate) {
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
    EXPECT_EQ(avl.getHeight(avl->right), 2);
    EXPECT_EQ(avl.getHeight(avl->left), 1);
    EXPECT_EQ(avl.getHeight(avl->right->right), 1);
}

TEST(AVLTree, can_bigRightRotate) {
    AVLTree<int, int> avl;
    avl.insert(30, 300);
    avl.insert(20, 200);
    avl.insert(10, 100);
    
    EXPECT_EQ(avl.getHeight(avl->left), 1);
    
    avl.insert(25, 250);
    avl.insert(5, 50);

    EXPECT_EQ(avl.getHeight(avl->left), 2);
    EXPECT_EQ(avl.getHeight(avl->left->left), 1);
    EXPECT_EQ(avl.getHeight(avl->right->left), 1);

    avl.remove(20);
    EXPECT_EQ(avl.getHeight(avl->left), 2);
    EXPECT_EQ(avl.getHeight(avl->left->left), 1);
    EXPECT_EQ(avl.getHeight(avl->right), 1);
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
	AVLTreeIterator<int, std::string> it_end = tree.end();
	EXPECT_EQ((*it_end), "seven");
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

TEST(AVLTree, can_insert_random_values) {
	AVLTree<int, int> tree;
	std::vector<int> randNums;
	for (int i = 0; i < 1000; i++) {
		randNums.push_back(i);
	}

	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randNums.size();
		tree.insert(randNums[ind], 1);
		std::swap(randNums[ind], randNums[randNums.size() - 1]);
		randNums.pop_back();
	}
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
}

TEST(AVLTree, can_insert_and_remove_random_values) {
	AVLTree<int, int> tree;
	std::vector<int> randNums;
	std::vector<int> randRem;
	srand(0);
	for (int i = 0; i < 1000; i++) {
		randNums.push_back(i);
		randRem.push_back(i);
	}

	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randNums.size();
		tree.insert(randNums[ind], 1);
		std::swap(randNums[ind], randNums[randNums.size() - 1]);
		randNums.pop_back();
	}
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randRem.size();
		tree.remove(randRem[ind]);
		std::swap(randRem[ind], randRem[randRem.size() - 1]);
		randRem.pop_back();
		EXPECT_EQ(tree.sizeTree(), 999-i);
	}
	EXPECT_EQ(tree.sizeTree(), 0);
}

TEST(AVLTree, iterator_works_with_changes_in_values_big_tree) {
	AVLTree<int, int> tree;
	for (int i = 0; i < 1000; i++) {
		tree.insert(i, i);
	}

	AVLTreeIterator<int, int> it = tree.begin();
	for (int i = 0; i < 1000; i++) {
		it->value += 1;
		++it;
	}
	AVLTreeIterator<int, int> check = tree.begin();
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ((*check), i + 1);
		++check;
	}

}

TEST(Binary_and_AVL_Trees, time_insert_random_values) {
	AVLTree<int, int> avl;
	BinaryTree<int, int> tree;
	const int N = 1000;
	std::vector<int> randInsert(N);
	std::vector<int> randRemove(N);
	std::iota(randInsert.begin(), randInsert.end(), 0);
	std::iota(randRemove.begin(), randRemove.end(), 0);
	std::mt19937 g(0);
	std::shuffle(randInsert.begin(), randInsert.end(), g);
	std::shuffle(randRemove.begin(), randRemove.end(), g);
	auto t_start_insert_bin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		tree.insert(randInsert[i], 1);
	}
	auto t_end_insert_bin = std::chrono::high_resolution_clock::now();

	auto t_start_insert_avl = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		avl.insert(randInsert[i], 1);
	}
	auto t_end_insert_avl = std::chrono::high_resolution_clock::now();

	auto t_start_find_bin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
	auto t_end_find_bin = std::chrono::high_resolution_clock::now();

	auto t_start_find_avl = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(avl.find(i)->value, 1);
	}
	auto t_end_find_avl = std::chrono::high_resolution_clock::now();

	auto t_start_remove_bin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		tree.remove(randRemove[i]);
	}
	auto t_end_remove_bin = std::chrono::high_resolution_clock::now();

	auto t_start_remove_avl = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		avl.remove(randRemove[i]);
	}
	auto t_end_remove_avl = std::chrono::high_resolution_clock::now();

	double insert_bin_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_insert_bin - t_start_insert_bin).count();
	double insert_avl_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_insert_avl - t_start_insert_avl).count();
	double find_bin_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_find_bin - t_start_find_bin).count();
	double find_avl_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_find_avl - t_start_find_avl).count();
	double remove_bin_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_remove_bin - t_start_remove_bin).count();
	double remove_avl_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_remove_avl - t_start_remove_avl).count();
	double ratio_insert = insert_avl_elapsed_time_ms / insert_bin_elapsed_time_ms;
	double ratio_find = find_avl_elapsed_time_ms / find_bin_elapsed_time_ms;
	double ratio_remove = remove_avl_elapsed_time_ms / remove_bin_elapsed_time_ms;

	std::cout << "Insertions time: binary tree: " << insert_bin_elapsed_time_ms << ", avl tree: " << insert_avl_elapsed_time_ms << std::endl;
	std::cout << "Binary tree is faster in " << ratio_insert << " times" << std::endl;
	std::cout << "Find time: binary tree: " << find_bin_elapsed_time_ms << ", avl tree: " << find_avl_elapsed_time_ms << std::endl;
	std::cout << "Binary tree is faster in " << ratio_find << " times" << std::endl;
	std::cout << "Remove time: binary tree: " << remove_bin_elapsed_time_ms << ", avl tree: " << remove_avl_elapsed_time_ms << std::endl;
	std::cout << "Binary tree is faster in " << ratio_remove << " times" << std::endl;
}

TEST(RBTree, can_insert_and_find) {
    RBTree<int, int> rbTree;
    rbTree.insert(10, 10);
    rbTree.insert(20, 20);
    rbTree.insert(30, 30);
    rbTree.insert(5, 5);
    rbTree.insert(15, 15);
    rbTree.insert(25, 25);
    rbTree.insert(35, 35);

    EXPECT_EQ(rbTree.find(10)->value, 10);
    EXPECT_EQ(rbTree.find(20)->value, 20);
    EXPECT_EQ(rbTree.find(30)->value, 30);
    EXPECT_EQ(rbTree.find(5)->value, 5);
    EXPECT_EQ(rbTree.find(15)->value, 15);
    EXPECT_EQ(rbTree.find(25)->value, 25);
    EXPECT_EQ(rbTree.find(35)->value, 35);
}

TEST(RBTree, can_get_size) {
    RBTree<int, int> rbTree;
    rbTree.insert(10, 10);
    rbTree.insert(20, 20);
    rbTree.insert(30, 30);
    rbTree.insert(5, 5);
    rbTree.insert(15, 15);
    rbTree.insert(25, 25);
    rbTree.insert(35, 35);

    EXPECT_EQ(rbTree.sizeTree(), 7);

    rbTree.remove(15);
    rbTree.remove(35);

    EXPECT_EQ(rbTree.sizeTree(), 5);
}

TEST(RBTree, can_remove) {
    RBTree<int, int> rbTree;
    rbTree.insert(10, 10);
    rbTree.insert(20, 20);
    rbTree.insert(30, 30);
    rbTree.insert(5, 5);
    rbTree.insert(15, 15);
    rbTree.insert(25, 25);
    rbTree.insert(35, 35);

    rbTree.remove(15);
    rbTree.remove(35);

    EXPECT_EQ(rbTree.find(15), nullptr);
    EXPECT_EQ(rbTree.find(35), nullptr);
    EXPECT_EQ(rbTree.sizeTree(), 5);
}

TEST(RBTree, can_get_begin_and_end) {
    RBTree<int, int> rbTree;
    rbTree.insert(10, 10);
    rbTree.insert(20, 20);
    rbTree.insert(30, 30);
    rbTree.insert(5, 5);
    rbTree.insert(15, 15);
    rbTree.insert(25, 25);
    rbTree.insert(35, 35);

    auto it = rbTree.begin();
    EXPECT_EQ(it->value, 5);

    it = rbTree.end();
    EXPECT_EQ(it->value, 35);
}

TEST(RBTree, iterator_works) {
	RBTree<int, std::string> tree;
	tree.insert(5, "five");
	tree.insert(3, "three");
	tree.insert(7, "seven");

	RBTreeIterator<int, std::string> it = tree.begin();
	EXPECT_EQ((*it), "three");
	++it;
	EXPECT_EQ((*it), "five");
	RBTreeIterator<int, std::string> it_end = tree.end();
	EXPECT_EQ((*it_end), "seven");
}

TEST(RBTree, iterator_works_with_changes_in_values) {
	RBTree<int, int> tree;
	tree.insert(5, 5);
	tree.insert(3, 3);
	tree.insert(7, 7);

	RBTreeIterator<int, int> it = tree.begin();
	it->value += 1;
	EXPECT_EQ((*it), 4);
	++it;
	it->value += 1;
	EXPECT_EQ((*it), 6);
	++it;
	it->value += 1;
	EXPECT_EQ((*it), 8);
}

TEST(RBTree, can_insert_random_values) {
	RBTree<int, int> tree;
	std::vector<int> randNums;
	for (int i = 0; i < 1000; i++) {
		randNums.push_back(i);
	}

	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randNums.size();
		tree.insert(randNums[ind], 1);
		std::swap(randNums[ind], randNums[randNums.size() - 1]);
		randNums.pop_back();
	}
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
}

TEST(RBTree, can_insert_and_remove_random_values) {
	RBTree<int, int> tree;
	std::vector<int> randNums;
	std::vector<int> randRem;
	srand(0);
	for (int i = 0; i < 1000; i++) {
		randNums.push_back(i);
		randRem.push_back(i);
	}

	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randNums.size();
		tree.insert(randNums[ind], 1);
		std::swap(randNums[ind], randNums[randNums.size() - 1]);
		randNums.pop_back();
	}
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
	for (int i = 0; i < 1000; i++) {
		int ind = rand() % randRem.size();
		tree.remove(randRem[ind]);
		std::swap(randRem[ind], randRem[randRem.size() - 1]);
		randRem.pop_back();
		EXPECT_EQ(tree.sizeTree(), 999-i);
	}
	EXPECT_EQ(tree.sizeTree(), 0);
}

TEST(RBTree, iterator_works_with_changes_in_values_big_tree) {
	RBTree<int, int> tree;
	for (int i = 0; i < 1000; i++) {
		tree.insert(i, i);
	}

	RBTreeIterator<int, int> it = tree.begin();
	for (int i = 0; i < 1000; i++) {
		it->value += 1;
		++it;
	}
	RBTreeIterator<int, int> check = tree.begin();
	for (int i = 0; i < 1000; i++) {
		EXPECT_EQ((*check), i + 1);
		++check;
	}

}

TEST(Binary_and_AVL_Trees_and_RBTree, time_insert_random_values) {
	RBTree<int,int> rb;
	AVLTree<int, int> avl;
	BinaryTree<int, int> tree;
	const int N = 1000;
	std::vector<int> randInsert(N);
	std::vector<int> randRemove(N);
	std::iota(randInsert.begin(), randInsert.end(), 0);
	std::iota(randRemove.begin(), randRemove.end(), 0);
	std::mt19937 g(0);
	std::shuffle(randInsert.begin(), randInsert.end(), g);
	std::shuffle(randRemove.begin(), randRemove.end(), g);
	auto t_start_insert_bin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		tree.insert(randInsert[i], 1);
	}
	auto t_end_insert_bin = std::chrono::high_resolution_clock::now();

	auto t_start_insert_avl = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		avl.insert(randInsert[i], 1);
	}
	auto t_end_insert_avl = std::chrono::high_resolution_clock::now();

	auto t_start_insert_rb = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		rb.insert(randInsert[i], 1);
	}
	auto t_end_insert_rb = std::chrono::high_resolution_clock::now();

	auto t_start_find_bin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(tree.find(i)->value, 1);
	}
	auto t_end_find_bin = std::chrono::high_resolution_clock::now();

	auto t_start_find_avl = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(avl.find(i)->value, 1);
	}
	auto t_end_find_avl = std::chrono::high_resolution_clock::now();

	auto t_start_find_rb = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(rb.find(i)->value, 1);
	}
	auto t_end_find_rb = std::chrono::high_resolution_clock::now();

	auto t_start_remove_bin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		tree.remove(randRemove[i]);
	}
	auto t_end_remove_bin = std::chrono::high_resolution_clock::now();

	auto t_start_remove_avl = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		avl.remove(randRemove[i]);
	}
	auto t_end_remove_avl = std::chrono::high_resolution_clock::now();

	auto t_start_remove_rb = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		rb.remove(randRemove[i]);
	}
	auto t_end_remove_rb = std::chrono::high_resolution_clock::now();

	double insert_bin_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_insert_bin - t_start_insert_bin).count();
	double insert_avl_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_insert_avl - t_start_insert_avl).count();
	double insert_rb_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_insert_rb - t_start_insert_rb).count();
	double find_bin_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_find_bin - t_start_find_bin).count();
	double find_avl_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_find_avl - t_start_find_avl).count();
	double find_rb_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_find_rb - t_start_find_rb).count();
	double remove_bin_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_remove_bin - t_start_remove_bin).count();
	double remove_avl_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_remove_avl - t_start_remove_avl).count();
	double remove_rb_elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end_remove_rb - t_start_remove_rb).count();
	//double ratio_insert = insert_avl_elapsed_time_ms / insert_bin_elapsed_time_ms;
	//double ratio_find = find_avl_elapsed_time_ms / find_bin_elapsed_time_ms;
	//double ratio_remove = remove_avl_elapsed_time_ms / remove_bin_elapsed_time_ms;

	std::cout << "Insertions time: binary tree: " << insert_bin_elapsed_time_ms << ", avl tree: " << insert_avl_elapsed_time_ms << ", rb tree" << insert_rb_elapsed_time_ms <<std::endl;
	//std::cout << "Binary tree is faster in " << ratio_insert << " times" << std::endl;
	std::cout << "Find time: binary tree: " << find_bin_elapsed_time_ms << ", avl tree: " << find_avl_elapsed_time_ms << ", rb tree" << find_rb_elapsed_time_ms << std::endl;
	//std::cout << "Binary tree is faster in " << ratio_find << " times" << std::endl;
	std::cout << "Remove time: binary tree: " << remove_bin_elapsed_time_ms << ", avl tree: " << remove_avl_elapsed_time_ms << ", rb tree" << remove_rb_elapsed_time_ms << std::endl;
	//std::cout << "Binary tree is faster in " << ratio_remove << " times" << std::endl;
}
