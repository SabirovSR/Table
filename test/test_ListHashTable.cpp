#include "../Table/Table.h"
#include "../Table/ListHashTable.h"

#include <string>
#include "gtest.h"


TEST(ListHashTable, InsertAndFind) {
    ListHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(5, "five"));
    EXPECT_TRUE(table.Find(5));
    EXPECT_EQ(table.getCurrVal(), "five");
    EXPECT_EQ(table.getCurrKey(), 5);
}

TEST(ListHashTable, FindNonexistentReturnsFalse) {
    ListHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_FALSE(table.Find(999));
}

TEST(ListHashTable, InsertDuplicateThrows) {
    ListHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(7, "seven"));
    EXPECT_THROW(table.Insert(Record<int, std::string>(7, "duplicate")), std::runtime_error);
}

TEST(ListHashTable, DeleteExistingKeyWorks) {
    ListHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(3, "three"));
    EXPECT_NO_THROW(table.Delete(3));
    EXPECT_FALSE(table.Find(3));
}

TEST(ListHashTable, DeleteNonexistentKeyThrows) {
    ListHashTable<int, std::string> table(10);
    EXPECT_THROW(table.Delete(42), std::runtime_error);
}

TEST(ListHashTable, IterationOverElements) {
    ListHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    table.Insert(Record<int, std::string>(2, "two"));

    table.Reset();
    int count = 0;
    while (!table.IsEnd()) {
        auto rec = table.getCurr();
        EXPECT_TRUE(rec.key == 1 || rec.key == 2);
        table.GoNext();
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(ListHashTable, FillTabWorksCorrectly) {
    ListHashTable<int, std::string> table(20);
    table.fillTab(10, 50);

    int count = 0;
    table.Reset();
    while (!table.IsEnd()) {
        count++;
        table.GoNext();
    }
    EXPECT_EQ(count, 10);
}

TEST(ListHashTable, ClearTableEmptiesAll) {
    ListHashTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(11, "eleven"));
    table.Insert(Record<int, std::string>(12, "twelve"));
    table.clrTab();

    int count = 0;
    table.Reset();
    while (!table.IsEnd()) {
        count++;
        table.GoNext();
    }
    EXPECT_EQ(count, 0);
}

TEST(ListHashTable, DataCountAccurate) {
    ListHashTable<int, std::string> table(10);
    EXPECT_EQ(table.getDataCount(), 0);
    table.Insert(Record<int, std::string>(9, "nine"));
    EXPECT_EQ(table.getDataCount(), 1);
    table.Insert(Record<int, std::string>(10, "ten"));
    EXPECT_EQ(table.getDataCount(), 2);
    table.Delete(9);
    EXPECT_EQ(table.getDataCount(), 1);
}

