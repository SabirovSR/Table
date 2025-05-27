#include "../Table/Table.h"
#include "../Table/TreeTable.h"

#include <string>
#include "gtest.h"

TEST(TreeTableTest, InsertAndFind) {
    TreeTable<int, std::string> tree;
    EXPECT_NO_THROW(tree.Insert(Record<int, std::string>(5, "five")));
    EXPECT_TRUE(tree.Find(5));
    EXPECT_EQ(tree.getCurrVal(), "five");
}

TEST(TreeTableTest, InsertDuplicateThrows) {
    TreeTable<int, std::string> tree;
    tree.Insert(Record<int, std::string>(1, "one"));
    EXPECT_THROW(tree.Insert(Record<int, std::string>(1, "one again")), std::runtime_error);
}

TEST(TreeTableTest, FindNonexistentReturnsFalse) {
    TreeTable<int, std::string> tree;
    tree.Insert(Record<int, std::string>(2, "two"));
    EXPECT_FALSE(tree.Find(99));
}

TEST(TreeTableTest, DeleteExistingKey) {
    TreeTable<int, std::string> tree;
    tree.Insert(Record<int, std::string>(10, "ten"));
    EXPECT_NO_THROW(tree.Delete(10));
    EXPECT_FALSE(tree.Find(10));
}

TEST(TreeTableTest, DeleteNonexistentSafe) {
    TreeTable<int, std::string> tree;
    EXPECT_NO_THROW(tree.Delete(404));
}

TEST(TreeTableTest, IterationWorks) {
    TreeTable<int, std::string> tree;
    tree.Insert(Record<int, std::string>(3, "three"));
    tree.Insert(Record<int, std::string>(1, "one"));
    tree.Insert(Record<int, std::string>(4, "four"));

    tree.Reset();
    int count = 0;
    while (!tree.IsEnd()) {
        count++;
        tree.GoNext();
    }
    EXPECT_EQ(count, 3);
}

TEST(TreeTableTest, ClearWorks) {
    TreeTable<int, std::string> tree;
    tree.Insert(Record<int, std::string>(5, "five"));
    tree.Insert(Record<int, std::string>(6, "six"));
    EXPECT_NO_THROW(tree.clrTab());
    EXPECT_EQ(tree.getDataCount(), 0);
}

TEST(TreeTableTest, FillTabCorrectly) {
    TreeTable<int, std::string> tree(50);
    EXPECT_NO_THROW(tree.fillTab(10, 50));
    int count = 0;
    tree.Reset();
    while (!tree.IsEnd()) {
        count++;
        tree.GoNext();
    }
    EXPECT_EQ(count, 10);
}

TEST(TreeTableTest, FillTabInvalidArgsThrow) {
    TreeTable<int, std::string> tree(5);
    EXPECT_THROW(tree.fillTab(0, 10), std::runtime_error);
    EXPECT_THROW(tree.fillTab(10, 10), std::runtime_error); 
}

TEST(TreeTableTest, GetCurrSafeAccess) {
    TreeTable<int, std::string> tree;
    tree.Insert(Record<int, std::string>(100, "val"));
    tree.Reset();
    EXPECT_EQ(tree.getCurrKey(), 100);
    EXPECT_EQ(tree.getCurrVal(), "val");

    tree.GoNext(); 
    EXPECT_EQ(tree.getCurrKey(), 0); 
    EXPECT_EQ(tree.getCurrVal(), ""); 
}

TEST(TreeTableTest, DataCountAccurate) {
    TreeTable<int, std::string> tree;
    EXPECT_EQ(tree.getDataCount(), 0);
    tree.Insert(Record<int, std::string>(1, "one"));
    tree.Insert(Record<int, std::string>(2, "two"));
    EXPECT_EQ(tree.getDataCount(), 2);
    tree.Delete(1);
    EXPECT_EQ(tree.getDataCount(), 1);
}