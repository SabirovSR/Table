#include "../Table/Table.h"
#include "../Table/ScanTable.h"

#include <string>
#include "gtest.h"

TEST(ScanTable, InsertAndFindElement) {
    ScanTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_TRUE(table.Find(1));
    EXPECT_EQ(table.getCurrVal(), "one");
    EXPECT_EQ(table.getCurrKey(), 1);
}

TEST(ScanTable, DeleteElementCorrectly) {
    ScanTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(2, "two"));
    table.Delete(2);
    EXPECT_FALSE(table.Find(2));
    EXPECT_EQ(table.getDataCount(), 0);
}

TEST(ScanTable, FindNonExistentKeyReturnsFalse) {
    ScanTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(3, "three"));
    EXPECT_FALSE(table.Find(999));
}

TEST(ScanTable, InsertDuplicateKeyThrows) {
    ScanTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(4, "four"));
    ASSERT_THROW(table.Insert(Record<int, std::string>(4, "duplicate")), std::runtime_error);
}

TEST(ScanTable, InsertFullThrows) {
    ScanTable<int, std::string> table(1);
    table.Insert(Record<int, std::string>(1, "one"));
    ASSERT_THROW(table.Insert(Record<int, std::string>(2, "two")), std::runtime_error);
    EXPECT_FALSE(table.Find(2));
    EXPECT_EQ(table.getDataCount(), 1);
}

TEST(ScanTable, IsEmptyAndIsFullStatuses) {
    ScanTable<int, std::string> table(2);
    EXPECT_TRUE(table.IsEmpty());
    EXPECT_FALSE(table.IsFull());

    table.Insert(Record<int, std::string>(1, "one"));
    table.Insert(Record<int, std::string>(2, "two"));

    EXPECT_TRUE(table.IsFull());
    EXPECT_FALSE(table.IsEmpty());
}

TEST(ScanTable, DataCountIncrementsAndDecrements) {
    ScanTable<int, std::string> table(10);
    EXPECT_EQ(table.getDataCount(), 0);
    table.Insert(Record<int, std::string>(1, "one"));
    EXPECT_EQ(table.getDataCount(), 1);
    table.Insert(Record<int, std::string>(2, "two"));
    EXPECT_EQ(table.getDataCount(), 2);
    table.Delete(1);
    EXPECT_EQ(table.getDataCount(), 1);
}

TEST(ScanTable, DeleteNonexistentKeyThrows) {
    ScanTable<int, std::string> table(10);
    ASSERT_THROW(table.Delete(42), std::runtime_error);
}

TEST(ScanTable, FillTableGeneratesUniqueKeys) {
    ScanTable<int, std::string> table(10);
    table.fillTab(5, 100);
    EXPECT_EQ(table.getDataCount(), 5);

    std::set<int> keys;
    for (int i = 0; i < 100; ++i) {
        if (table.Find(i)) {
            keys.insert(table.getCurrKey());
        }
    }
    EXPECT_EQ(keys.size(), 5);
}

TEST(ScanTable, ClearTableResetsTable) {
    ScanTable<int, std::string> table(10);
    table.Insert(Record<int, std::string>(1, "one"));
    table.Insert(Record<int, std::string>(2, "two"));
    table.clrTab();
    EXPECT_TRUE(table.IsEmpty());
    EXPECT_EQ(table.getDataCount(), 0);
}