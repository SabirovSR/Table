#include "../Table/Table.h"
#include "../Table/ArrayHashTable.h"

#include <string>
#include "gtest.h"



TEST(ArrayHashTable, InsertAndFindElement) {
    ArrayHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_TRUE(table.Find(1));
    EXPECT_EQ(table.getCurrVal(), "one");
    EXPECT_EQ(table.getCurrKey(), 1);
}

TEST(ArrayHashTable, DeleteElementCorrectly) {
    ArrayHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(2, "two"));
    table.Delete(2);
    EXPECT_FALSE(table.Find(2));
}

TEST(ArrayHashTable, FindNonExistentKey) {
    ArrayHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(3, "three"));
    EXPECT_FALSE(table.Find(999));
}

TEST(ArrayHashTable, InsertDuplicateThrows) {
    ArrayHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(4, "four"));
    EXPECT_THROW(table.Insert(Record<int, std::string>(4, "duplicate")), std::runtime_error);
}

TEST(ArrayHashTable, InsertIntoFullTableThrows) {
    ArrayHashTable<int, std::string> table(1);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_THROW(table.Insert(Record<int, std::string>(2, "two")), std::runtime_error);
    EXPECT_FALSE(table.Find(2));
}

TEST(ArrayHashTable, IsEmptyAndIsFull) {
    ArrayHashTable<int, std::string> table(2);
    EXPECT_FALSE(table.IsFull());
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_FALSE(table.IsFull());
    table.Insert(Record<int, std::string>(2, "two"));
    EXPECT_TRUE(table.IsFull());
}

TEST(ArrayHashTable, DeleteNonexistentKeyThrows) {
    ArrayHashTable<int, std::string> table(5);
    EXPECT_THROW(table.Delete(999), const char*);
}

TEST(ArrayHashTable, DataCountUpdatesCorrectly) {
    ArrayHashTable<int, std::string> table(10);
    EXPECT_EQ(table.getDataCount(), 0);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_EQ(table.getDataCount(), 1);
    table.Insert(Record<int, std::string>(2, "two"));
    EXPECT_EQ(table.getDataCount(), 2);
    table.Delete(2);
    EXPECT_EQ(table.getDataCount(), 1);
}

TEST(ArrayHashTable, FillTabFillsCorrectly) {
    ArrayHashTable<int, std::string> table(10);
    table.fillTab(5, 50);
    int count = 0;
    table.Reset();
    while (!table.IsEnd()) {
        count++;
        table.GoNext();
    }
    EXPECT_EQ(count, 5);
}

TEST(ArrayHashTable, ClearTableWorks) {
    ArrayHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    table.Insert(Record<int, std::string>(2, "two"));
    table.clrTab();
    EXPECT_EQ(table.getDataCount(), 0);
    EXPECT_FALSE(table.Find(1));
    EXPECT_FALSE(table.Find(2));
}