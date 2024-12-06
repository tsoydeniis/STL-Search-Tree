#include <gtest/gtest.h>
#include "src/search_tree.h"

#include <algorithm>
#include <vector>
#include <numeric>
#include <random>
#include <set>




TEST(SearchTreeTest, InOrderTraversal) {
    SearchTree<int, in_order_tag> tree;

    // Вставляем элементы
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);
    tree.insert(35);

    std::vector<int> expected = { 5, 10, 15, 20, 25, 30, 35 };
    std::vector<int> result;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}
TEST(SearchTreeTest, PreOrderTraversal) {
    SearchTree<int, pre_order_tag> tree;

    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);
    tree.insert(35);

    std::vector<int> expected = { 20, 10, 5, 15, 30, 25, 35 };
    std::vector<int> result;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

TEST(SearchTreeTest, PostOrderTraversal) {
    SearchTree<int, post_order_tag> tree;

    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);
    tree.insert(35);

    std::vector<int> expected = { 5, 15, 10, 25, 35, 30, 20 };
    std::vector<int> result;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}



TEST(SearchTreeTest, InsertAndFind) {
    SearchTree<int, in_order_tag> tree;

    auto [it, inserted] = tree.insert(10);
    EXPECT_TRUE(inserted);
    EXPECT_EQ(*it, 10);

    it = tree.find(10);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(*it, 10);

    it = tree.find(5);
    EXPECT_EQ(it, tree.end()); 
}

TEST(SearchTreeTest, InsertDuplicate) {
    SearchTree<int, in_order_tag> tree;

    tree.insert(10);
    auto [it, inserted] = tree.insert(10);
    EXPECT_FALSE(inserted);
    EXPECT_EQ(*it, 10);
}

TEST(SearchTreeTest, InsertRange) {
    SearchTree<int, in_order_tag> tree;
    std::vector<int> data = { 10, 20, 30, 40 };

    tree.insert(data.begin(), data.end());

    EXPECT_EQ(tree.size(), data.size());

    auto it = tree.begin();
    EXPECT_EQ(*it++, 10);
    EXPECT_EQ(*it++, 20);
    EXPECT_EQ(*it++, 30);
    EXPECT_EQ(*it, 40);
}

TEST(SearchTreeTest, EraseSingleElement) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);

    EXPECT_EQ(tree.size(), 2);

    size_t erased = tree.erase(10);
    EXPECT_EQ(erased, 1);
    EXPECT_EQ(tree.size(), 1);

    auto it = tree.find(10);
    EXPECT_EQ(it, tree.end());  
}

TEST(SearchTreeTest, ExtractElement) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);

    auto* node = tree.extract(10);
    EXPECT_EQ(node->value, 10);
    EXPECT_EQ(tree.size(), 1);

    auto it = tree.find(10);
    EXPECT_EQ(it, tree.end());  
}

TEST(SearchTreeTest, IteratorTraversal) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    auto it = tree.begin();
    EXPECT_EQ(*it++, 5);
    EXPECT_EQ(*it++, 10);
    EXPECT_EQ(*it, 15);

    it = tree.end();
    --it;
    EXPECT_EQ(*it, 15);
}

TEST(SearchTreeTest, LowerBound) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    auto it = tree.lower_bound(10);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(*it, 10);

    it = tree.lower_bound(12);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(*it, 15);

    it = tree.lower_bound(20);
    EXPECT_EQ(it, tree.end());
}

TEST(SearchTreeTest, UpperBound) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    auto it = tree.upper_bound(10);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(*it, 15);

    it = tree.upper_bound(15);
    EXPECT_EQ(it, tree.end());
}

TEST(SearchTreeTest, ReverseIteratorTraversal) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    auto rit = tree.rbegin();
    EXPECT_EQ(*rit++, 15);
    EXPECT_EQ(*rit++, 10);
    EXPECT_EQ(*rit, 5);

    rit = tree.rend();
    --rit;
    EXPECT_EQ(*rit, 5);
}

TEST(SearchTreeTest, ClearAndEmpty) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    tree.clear();
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.begin(), tree.end());
}

TEST(SearchTreeTest, MaxSize) {
    SearchTree<int, in_order_tag> tree;
    EXPECT_GT(tree.max_size(), 0); 
}

TEST(SearchTreeTest, Swap) {
    SearchTree<int, in_order_tag> tree1;
    tree1.insert(10);
    tree1.insert(5);

    SearchTree<int, in_order_tag> tree2;
    tree2.insert(15);
    tree2.insert(20);

    tree1.swap(tree2);

    EXPECT_EQ(tree1.size(), 2);
    EXPECT_EQ(tree2.size(), 2);

    auto it = tree1.begin();
    EXPECT_EQ(*it++, 15);
    EXPECT_EQ(*it, 20);

    it = tree2.begin();
    EXPECT_EQ(*it++, 5);
    EXPECT_EQ(*it, 10);
}

TEST(SearchTreeTest, EqualRange) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    auto [lower, upper] = tree.equal_range(10);
    EXPECT_EQ(*lower, 10);
    EXPECT_EQ(*upper, 15);

    auto [lower2, upper2] = tree.equal_range(12);
    EXPECT_EQ(*lower2, 15);
    EXPECT_EQ(*upper2, 15);
}

TEST(SearchTreeTest, KeyCompare) {
    SearchTree<int, in_order_tag> tree;
    auto comp = tree.key_comp();

    EXPECT_TRUE(comp(5, 10));
    EXPECT_FALSE(comp(10, 5));
}

TEST(SearchTreeTest, ValueCompare) {
    SearchTree<int, in_order_tag> tree;
    auto comp = tree.value_comp();

    EXPECT_TRUE(comp(5, 10));
    EXPECT_FALSE(comp(10, 5));
}


TEST(InOrderTraversal, ComplexTest) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(8);
    tree.insert(3);
    tree.insert(10);
    tree.insert(1);
    tree.insert(6);
    tree.insert(4);
    tree.insert(7);
    tree.insert(14);
    tree.insert(13);

    std::vector<int> expected = { 1, 3, 4, 6, 7, 8, 10, 13, 14 };
    std::vector<int> result;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}
TEST(PreOrderTraversal, ComplexTest) {
    SearchTree<int, pre_order_tag> tree;
    tree.insert(8);
    tree.insert(3);
    tree.insert(10);
    tree.insert(1);
    tree.insert(6);
    tree.insert(4);
    tree.insert(7);
    tree.insert(14);
    tree.insert(13);

    std::vector<int> expected = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
    std::vector<int> result;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}
TEST(PostOrderTraversal, ComplexTest) {
    SearchTree<int, post_order_tag> tree;
    tree.insert(8);
    tree.insert(3);
    tree.insert(10);
    tree.insert(1);
    tree.insert(6);
    tree.insert(4);
    tree.insert(7);
    tree.insert(14);
    tree.insert(13);

    std::vector<int> expected = { 1, 4, 7, 6, 3, 13, 14, 10, 8 };
    std::vector<int> result;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}


TEST(STLCompatibility, STLAlgorithmTest) {
    SearchTree<int, in_order_tag> tree;
    tree.insert(8);
    tree.insert(3);
    tree.insert(10);
    tree.insert(1);
    tree.insert(6);
    tree.insert(4);
    tree.insert(7);
    tree.insert(14);
    tree.insert(13);

    auto it = std::find(tree.begin(), tree.end(), 7);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(*it, 7);

    int count = std::count_if(tree.begin(), tree.end(), [](int x) { return x > 5; });
    EXPECT_EQ(count, 6); 

    int sum = std::accumulate(tree.begin(), tree.end(), 0);
    EXPECT_EQ(sum, 66);

    auto [lower, upper] = std::equal_range(tree.begin(), tree.end(), 7);
    EXPECT_EQ(*lower, 7); 
    EXPECT_EQ(*upper, 8);

    auto min_it = std::min_element(tree.begin(), tree.end());
    auto max_it = std::max_element(tree.begin(), tree.end());
    EXPECT_EQ(*min_it, 1); 
    EXPECT_EQ(*max_it, 14);
}


TEST(SearchTree, LargeRandomTest) {
    const int num_elements = 1000000;  // количество случайных элементов
    std::vector<int> random_numbers(num_elements);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, num_elements * 10);


    std::generate(random_numbers.begin(), random_numbers.end(), [&]() {return distrib(gen);});

    SearchTree<int, in_order_tag> tree;
    std::set<int> std_set;

    for (int num : random_numbers) {
        tree.insert(num);
        std_set.insert(num);
    }

    EXPECT_EQ(tree.size(), std_set.size());

    auto tree_it = tree.begin();
    auto set_it = std_set.begin();

    while (tree_it != tree.end() && set_it != std_set.end()) {
        EXPECT_EQ(*tree_it, *set_it);
        ++tree_it;
        ++set_it;
    }

    EXPECT_EQ(tree_it, tree.end());
    EXPECT_EQ(set_it, std_set.end());

    for (int i = 0; i < 10000; ++i) {
        int random_value = distrib(gen);
        auto tree_find = tree.find(random_value);
        auto set_find = std_set.find(random_value);

        if (set_find != std_set.end()) {
            EXPECT_NE(tree_find, tree.end());
            EXPECT_EQ(*tree_find, *set_find);
        }
        else {
            EXPECT_EQ(tree_find, tree.end());
        }
    }

    for (int i = 0; i < 10000; ++i) {
        int random_value = distrib(gen);

        auto tree_lower = tree.lower_bound(random_value);
        auto set_lower = std_set.lower_bound(random_value);
        if (set_lower != std_set.end()) {
            EXPECT_NE(tree_lower, tree.end());
            EXPECT_EQ(*tree_lower, *set_lower);
        }
        else {
            EXPECT_EQ(tree_lower, tree.end());
        }

        auto tree_upper = tree.upper_bound(random_value);
        auto set_upper = std_set.upper_bound(random_value);
        if (set_upper != std_set.end()) {
            EXPECT_NE(tree_upper, tree.end());
            EXPECT_EQ(*tree_upper, *set_upper);
        }
        else {
            EXPECT_EQ(tree_upper, tree.end());
        }
    }
}