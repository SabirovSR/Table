#include "../Table/Table.h"
#include "../Table/SortTable.h"

#include <string>
#include "gtest.h"

TEST(SortTable, InsertAndFindElement) {
    SortTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_TRUE(table.Find(1));
    EXPECT_EQ(table.getCurrVal(), "one");
    EXPECT_EQ(table.getCurrKey(), 1);
}

TEST(SortTable, DeleteElementCorrectly) {
    SortTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(2, "two"));
    table.Delete(2);
    EXPECT_FALSE(table.Find(2));
    EXPECT_EQ(table.getDataCount(), 0);
}

TEST(SortTable, FindNonExistentKeyReturnsFalse) {
    SortTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(3, "three"));
    EXPECT_FALSE(table.Find(999));
}

TEST(SortTable, InsertDuplicateKeyThrows) {
    SortTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(4, "four"));
    ASSERT_THROW(table.Insert(Record<int, std::string>(4, "duplicate")), std::runtime_error);
}

TEST(SortTable, InsertFullThrows) {
    SortTable<int, std::string> table(1);
    table.Insert(Record<int, std::string>(1, "one"));
    ASSERT_THROW(table.Insert(Record<int, std::string>(2, "two")), std::runtime_error);
    EXPECT_FALSE(table.Find(2));
    EXPECT_EQ(table.getDataCount(), 1);
}

TEST(SortTable, IsEmptyAndIsFullStatuses) {
    SortTable<int, std::string> table(2);
    EXPECT_TRUE(table.IsEmpty());
    EXPECT_FALSE(table.IsFull());

    table.Insert(Record<int, std::string>(1, "one"));
    table.Insert(Record<int, std::string>(2, "two"));

    EXPECT_TRUE(table.IsFull());
    EXPECT_FALSE(table.IsEmpty());
}

TEST(SortTable, DataCountIncrementsAndDecrements) {
    SortTable<int, std::string> table(10);
    EXPECT_EQ(table.getDataCount(), 0);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_EQ(table.getDataCount(), 1);
    table.Insert(Record<int, std::string>(2, "two"));
    EXPECT_EQ(table.getDataCount(), 2);
    table.Delete(1);
    EXPECT_EQ(table.getDataCount(), 1);
}

TEST(SortTable, DeleteNonexistentKeyThrows) {
    SortTable<int, std::string> table(10);
    ASSERT_THROW(table.Delete(42), std::runtime_error);
}

TEST(SortTable, MaintainsSortedOrderOnInsertion) {
    SortTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(20, "a"));
    table.Insert(Record<int, std::string>(10, "b"));
    table.Insert(Record<int, std::string>(30, "c"));
    table.Insert(Record<int, std::string>(25, "d"));

    std::vector<int> extracted_keys;
    for (int i = 0; i < 100; i++) {
        if (table.Find(i)) {
            extracted_keys.push_back(table.getCurrKey());
        }
    }

    EXPECT_TRUE(std::is_sorted(extracted_keys.begin(), extracted_keys.end()));
}

TEST(SortTable, MultipleDeletionsMaintainSortedOrder) {
    SortTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(5, "a"));
    table.Insert(Record<int, std::string>(10, "b"));
    table.Insert(Record<int, std::string>(15, "c"));
    table.Insert(Record<int, std::string>(20, "d"));

    table.Delete(10);
    table.Delete(5);

    std::vector<int> keys_after_deletion;
    for (int i = 0; i < 100; i++) {
        if (table.Find(i)) {
            keys_after_deletion.push_back(table.getCurrKey());
        }
    }

    EXPECT_EQ(keys_after_deletion.size(), 2);
    EXPECT_TRUE(std::is_sorted(keys_after_deletion.begin(), keys_after_deletion.end()));
}