#include <gtest/gtest.h>
#include "../src/core/utils/sparse_set.h"

struct TestType
{
    int32_t x;
    int32_t y;

    bool operator==(const TestType& other) const { return x == other.x && y == other.y; }
};

template<typename T>
static void assertSetsDataPointersDifferentAndNotNull(const SparseSet<T>& s1, const SparseSet<T>& s2)
{
    ASSERT_NE(s1.indices_data(), nullptr);
    ASSERT_NE(s1.data(), nullptr);
    ASSERT_NE(s2.indices_data(), nullptr);
    ASSERT_NE(s2.data(), nullptr);
    ASSERT_NE(s1.indices_data(), s2.indices_data());
    ASSERT_NE(s1.data(), s2.data());
}

TEST(SPARSE_SET, SizeConstructor_AfterConstructing_Data_Pointers_Are_Not_Null)
{
    SparseSet<TestType> s { 1 };

    ASSERT_TRUE(s.indices_data() != nullptr);
    ASSERT_TRUE(s.data() != nullptr);
}

TEST(SPARSE_SET, SizeConstructor_AfterConstructing_EmptyAndSizeIsZero)
{
    SparseSet<TestType> s { 1 };
    ASSERT_TRUE(s.empty() && s.size() == 0);
}

TEST(SPARSE_SET, SizeConstructor_AfterConstructing_CapacityAsExpected)
{
    const auto CAPACITY = 10;
    SparseSet<TestType> s { CAPACITY };
    ASSERT_EQ(s.capacity(), CAPACITY);
}

TEST(SPARSE_SET, CopyConstructor_AfterAssignmentToOtherVariable_DataPointersAreDifferentAndValid)
{
    SparseSet<TestType> s1 { 1 };
    SparseSet s2 { s1 };

    assertSetsDataPointersDifferentAndNotNull(s1, s2);
}

TEST(SPARSE_SET, AssignmentOperator_AfterSelfAssignment_SetsDataPointersAreSameAndNotNull)
{
    SparseSet<TestType> s1 { 1 };

    const auto* expectedSparseData = s1.indices_data();
    const auto* expectedDenseData = s1.data();

    s1 = s1;

    ASSERT_NE(s1.indices_data(), nullptr);
    ASSERT_NE(s1.data(), nullptr);
    ASSERT_EQ(s1.indices_data(), expectedSparseData);
    ASSERT_EQ(s1.data(), expectedDenseData);
}

TEST(SPARSE_SET, reserve_WithLesserCapacity_ThrowsException)
{
    const auto CAPACITY = 10;
    SparseSet<TestType> s { 10 };
    ASSERT_THROW(s.reserve(CAPACITY-1), std::runtime_error);
}

TEST(SPARSE_SET, reserve_WithSameCapacity_ThrowsException)
{
    const auto CAPACITY = 10;
    SparseSet<TestType> s { CAPACITY };
    ASSERT_THROW(s.reserve(CAPACITY), std::runtime_error);
}

TEST(SPARSE_SET, reserve_WithGreaterCapacity_IncreasesCapacityAndLeftZeroSize)
{
    const auto CAPACITY = 10;
    const auto EXPECTED_CAPACITY = CAPACITY + 1;
    const auto EXPECTED_SIZE = 0;
    SparseSet<TestType> s { CAPACITY };

    s.reserve(EXPECTED_CAPACITY);

    ASSERT_EQ(s.capacity(), EXPECTED_CAPACITY);
    ASSERT_EQ(s.size(), EXPECTED_SIZE);
}

TEST(SPARSE_SET, fast_insert_WhenFitToCapacity_IncreasesSizeAndAddedCorrectlyElement)
{
    SparseSet<TestType> s { 2 };
    const auto elem = TestType { 1,2 };
    auto idx = s.insert_fast(elem);

    ASSERT_EQ(idx, 0);
    ASSERT_EQ(s.size(), 1);
    ASSERT_EQ(*s.data(), elem);
}

TEST(SPARSE_SET, fast_insert_WhenCapacityIsZero_IncreasesSizeAndCapacityAndAddedCorrectlyElement)
{
    SparseSet<TestType> s;
    const auto elem = TestType { 1,2 };
    auto idx = s.insert_fast(elem);

    ASSERT_EQ(idx, 0);
    ASSERT_EQ(s.size(), 1);
    ASSERT_EQ(*s.data(), elem);
}

TEST(SPARSE_SET, contains_WhenInsertElement_ReturnsTrueIfElementExistsByIndexAndFalseIfNot)
{
    SparseSet<TestType> s;
    auto idx = s.insert_fast(TestType {1, 2});

    ASSERT_TRUE(s.contains(idx));
    ASSERT_FALSE(s.contains(idx + 1));
}

TEST(SPARSE_SET, get_WhenInsertElement_ReturnsElementOrThrowsExceptionIfNotExist)
{
    SparseSet<TestType> s;
    const auto& expectedElement = TestType { 1,2 };
    auto idx = s.insert_fast(expectedElement);

    std::vector<float> v;

    auto actual = s.get(idx);
    ASSERT_EQ(actual, expectedElement);
    ASSERT_THROW(s.get(idx + 1), std::runtime_error);
}

TEST(SPARSE_SET, remove_WhenInsertElement_ContainsReturnsFalseForGivenIndexAndSizeDecrease)
{
    SparseSet<TestType> s;
    auto idx = s.insert_fast(TestType {1, 2});
    s.remove(idx);

    ASSERT_FALSE(s.contains(idx));
    ASSERT_EQ(s.size(), 0);
    ASSERT_EQ(s.indices_size(), 0);
}

TEST(SPARSE_SET, remove_fromCenterDoesntViolateOtherData)
{
    SparseSet<TestType> s;
    const auto t1 = TestType { 1 };
    const auto t2 = TestType { 2 };
    const auto t3 = TestType { 3 };

    auto i1 = s.insert_fast(t1);
    auto i2 = s.insert_fast(t2);
    auto i3 = s.insert_fast(t3);

    s.remove(i2);

    ASSERT_TRUE(s.contains(i1));
    ASSERT_FALSE(s.contains(i2));
    ASSERT_TRUE(s.contains(i3));
    ASSERT_EQ(s.size(), 2);
    ASSERT_EQ(s.indices_size(), 3);
    ASSERT_EQ(s.get(i1), t1);
    ASSERT_EQ(s.get(i3), t3);
}