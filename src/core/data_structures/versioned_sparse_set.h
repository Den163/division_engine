#pragma once

#include <cstdint>
#include <string>

#include "sparse_set.h"

struct VersionedIndex
{
    uint32_t version;
    uint32_t index;

    static VersionedIndex null() { return VersionedIndex { 0, SPARSE_SET_NULL_INDEX }; }

    std::string to_string() const
    {
        return "{ version: " + std::to_string(version) + ", index: " + std::to_string(index) + " }";
    }
};

template< typename T,
          typename VersionAllocator = std::allocator<uint32_t>,
          typename SparseAllocator = std::allocator<uint32_t>,
          typename DenseAllocator = std::allocator<DenseElement<T>> >
class VersionedSparseSet
{
    using sparse_set = SparseSet<T, SparseAllocator, DenseAllocator>;

public:
    explicit VersionedSparseSet(
        uint32_t capacity = 0,
        VersionAllocator versionAllocator = VersionAllocator {},
        SparseAllocator sparseAllocator = SparseAllocator {},
        DenseAllocator denseAllocator = DenseAllocator {}
    ) :
        sparseSet_(sparse_set { capacity, sparseAllocator, denseAllocator }),
        versionAllocator_(versionAllocator)
    {
        versions_ = versionAllocator_.allocate(capacity);
    }

    VersionedSparseSet(VersionedSparseSet&&) noexcept = default;
    VersionedSparseSet(const VersionedSparseSet& other)
    {
        copy(other, *this);
    }

    VersionedSparseSet& operator=(const VersionedSparseSet& other)
    {
        if (this == &other) return *this;

        versionAllocator_.deallocate(versions_, capacity());
        copy(other, *this);
        return *this;
    }

    ~VersionedSparseSet()
    {
        versionAllocator_.deallocate(versions_, capacity());
    }

    VersionedIndex push_back(const T& element)
    {
        return push_back(std::move(element));
    }

    VersionedIndex push_back(const T&& element)
    {
        auto newSparseSize = sparseSet_.indices_size() + 1;
        if (newSparseSize > sparseSet_.capacity())
        {
            reserve(newSparseSize * 2);
        }

        auto sparseIndex = sparseSet_.push_back(element);
        versions_[sparseIndex] = 0;
        return VersionedIndex { .version = 0, .index = sparseIndex };
    }

    VersionedIndex insert(const T& element)
    {
        return insert(std::move(element));
    }

    VersionedIndex insert(const T&& element)
    {
        auto capacityBefore = sparseSet_.capacity();
        auto index = sparseSet_.insert(element);
        auto capacityAfter = sparseSet_.capacity();

        if (capacityBefore != capacityAfter)
        {
            reserve_internal(sparseSet_.capacity());
        }

        auto& version = versions_[index];

        return VersionedIndex { .version = ++version, .index = index };
    }

    T& get(const VersionedIndex& versionedIndex)
    {
        if (!contains(versionedIndex))
        {
            throw_no_index(versionedIndex);
        }

        return sparseSet_.get(versionedIndex.index);
    }

    void remove(const VersionedIndex& versionedIndex)
    {
        if (!contains(versionedIndex))
        {
            throw_no_index(versionedIndex);
        }

        sparseSet_.remove(versionedIndex.index);
    }

    void reserve(uint32_t newCapacity)
    {
        reserve_internal(newCapacity);
        sparseSet_.reserve(newCapacity);
    }

    bool contains(const VersionedIndex& versionedIndex) const
    {
        auto idx = versionedIndex.index;
        return sparseSet_.contains(idx) && (versions_[idx] == versionedIndex.version);
    }

    typename sparse_set::iterator begin() { return sparseSet_.begin(); }
    typename sparse_set::iterator end() { return sparseSet_.end(); }

    inline uint32_t capacity() const { return sparseSet_.capacity(); }
    inline uint32_t size() const { return sparseSet_.size(); }
    inline uint32_t indices_size() const { return sparseSet_.indices_size(); }
    inline bool empty() const { return sparseSet_.empty(); }
    inline const uint32_t* indices_data() const { return sparseSet_.indices_data(); }
    inline const DenseElement<T>* data() const { return sparseSet_.data(); }

    inline const uint32_t* versions_data() const { return versions_; }
    inline uint32_t versions_size() const { return indices_size(); }

    static constexpr inline uint32_t null_index() { return sparse_set::null_index(); }
    static constexpr inline uint32_t max_index() { return sparse_set::max_index(); }

private:
    static void throw_no_index(const VersionedIndex& index)
    {
        throw std::runtime_error{"There is no elements by a given versioned index: " + index.to_string()};
    }

    static inline void copy(const VersionedSparseSet& from, VersionedSparseSet& to)
    {
        to.sparseSet_ = from.sparseSet_;
        to.versionAllocator_ = from.versionAllocator_;

        to.versions_ = to.versionAllocator_.allocate(to.sparseSet_.capacity());
        std::copy(from.versions_, from.versions_ + from.sparseSet_.indices_size(), to.versions_);
    }

    void reserve_internal(uint32_t newCapacity)
    {
        auto capacityBefore = sparseSet_.capacity();
        auto sizeToCopy = sparseSet_.indices_size();
        auto* newVersions = versionAllocator_.allocate(newCapacity);

        std::copy(versions_, versions_ + sizeToCopy, newVersions);
        versionAllocator_.deallocate(versions_, capacityBefore);
        versions_ = newVersions;
    }

private:
    SparseSet<T, SparseAllocator, DenseAllocator> sparseSet_;
    VersionAllocator versionAllocator_;
    uint32_t* versions_;
};