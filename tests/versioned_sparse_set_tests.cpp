#include <gtest/gtest.h>
#include "test_type.h"
#include "../src/core/data_structures/versioned_sparse_set.h"

using TestVersionedSparseSet = typename VersionedSparseSet<TestType>;

TEST(VERSIONED_SPARSE_SET, SizeConstructor_AfterConstructingWithNonZeroSize_DataPointersAreNotNull)
{
    TestVersionedSparseSet s { 1 };

    ASSERT_NE(s.versions_data(), nullptr);
}

TEST(VERSIONED_SPARSE_SET, contains_AfterInsertReturnsTrue)
{
    TestVersionedSparseSet s { 1 };
    auto ver1 = s.push_back(TestType {1, 1});
    auto ver2 = s.push_back(TestType {1, 1});
    auto ver3 = s.push_back(TestType {1, 1});

    ASSERT_TRUE(s.contains(ver1));
    ASSERT_TRUE(s.contains(ver2));
    ASSERT_TRUE(s.contains(ver3));
}

TEST(VERSIONED_SPARSE_SET, remove_AfterInserting_ContainsReturnsAsExpected)
{
    TestVersionedSparseSet s { 1 };
    auto ver1 = s.push_back(TestType{1,1});
    auto ver2 = s.push_back(TestType{1,1});
    auto ver3 = s.push_back(TestType{1,1});
    s.remove(ver2);

    ASSERT_TRUE(s.contains(ver1));
    ASSERT_FALSE(s.contains(ver2));
    ASSERT_TRUE(s.contains(ver3));
}

TEST(VERSIONED_SPARSE_SET, get_ReturnsValuesAsExpected)
{
    TestVersionedSparseSet s { 1 };

    auto t1 = TestType{1};
    auto t2 = TestType{2};
    auto t3 = TestType{3};
    auto ver1 = s.push_back(t1);
    auto ver2 = s.push_back(t2);
    auto ver3 = s.push_back(t3);

    s.remove(ver2);

    ASSERT_EQ(t1, s.get(ver1));
    ASSERT_EQ(t3, s.get(ver3));
}

TEST(VERSIONED_SPARSE_SET, insert_AfterInsertedToRemoved_VersionIncreasedAndContainsReturnsFalse)
{
    TestVersionedSparseSet s { 1 };

    auto t1 = TestType{1};
    auto t2 = TestType{2};
    auto t3 = TestType{3};

    auto ver1 = s.insert(t1);
    auto ver2_1 = s.insert(t2);
    auto ver3 = s.insert(t3);
    s.remove(ver2_1);
    auto ver2_2 = s.insert(t2);

    ASSERT_TRUE(s.contains(ver1));
    ASSERT_FALSE(s.contains(ver2_1));
    ASSERT_TRUE(s.contains(ver3));
    ASSERT_TRUE(s.contains(ver2_2));

    ASSERT_EQ(t1, s.get(ver1));
    ASSERT_EQ(t2, s.get(ver2_2));
    ASSERT_EQ(t3, s.get(ver3));
}