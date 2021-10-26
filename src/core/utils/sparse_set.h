#pragma once

#include <vector>
#include <memory>
#include <concepts>
#include <thread>

template<typename TAlloc, typename T>
concept AllocatorType = requires(TAlloc alloc, const size_t n, T* memPtr)
{
    { alloc.allocate(n) } -> std::same_as<T*>;
    { alloc.deallocate(memPtr, n) };
};

template< typename T,
          AllocatorType<uint32_t> TSparseAlloc = std::allocator<uint32_t>,
          AllocatorType<T> TDenseAlloc = std::allocator<T> >
class SparseSet
{
private:
    using sparse_allocator = typename std::allocator_traits<TSparseAlloc>::allocator_type;
    using dense_allocator = typename std::allocator_traits<TDenseAlloc>::allocator_type;

    static constexpr size_t NULL_INDEX = std::numeric_limits<uint32_t>::max();

public:
    explicit SparseSet(size_t capacity = 0) :
        sparse_size_(0), dense_size_(0), capacity_(capacity), sparseAlloc_(TSparseAlloc{}), denseAlloc_(TDenseAlloc{})
    {
        sparse_ = sparseAlloc_.allocate(capacity);
        dense_ = denseAlloc_.allocate(capacity);
    }

    SparseSet(SparseSet&& sparseSet) noexcept = default;

    SparseSet(const SparseSet& other)
    {
        capacity_ = 0;
        copy(other, *this);
    }

    SparseSet& operator=(const SparseSet& other)
    {
        if (this == &other) return *this;

        copy(other, *this);
        return *this;
    }

    ~SparseSet()
    {
        sparseAlloc_.deallocate(sparse_, capacity_);
        denseAlloc_.deallocate(dense_, capacity_);
    }

    /*
     * Push next element and returns index associated with it. Doesn't try to do indices defragmentation
     */
    uint32_t insert_fast(const T& element)
    {
        return insert_fast(std::move(element));
    }

    uint32_t insert_fast(const T&& element)
    {
        auto newSparseSize = sparse_size_ + 1;
        if (newSparseSize > capacity_)
        {
            reserve(newSparseSize * 2);
        }

        auto nextSparseIdx = sparse_size_;
        auto nextDenseIdx = dense_size_;

        if (nextDenseIdx == NULL_INDEX)
        {
            throw std::runtime_error { "Exceed limit of the maximum element index: " + std::to_string(max_index()) };
        }

        dense_[nextDenseIdx] = element;
        sparse_[nextSparseIdx] = nextDenseIdx;

        sparse_size_ = newSparseSize;
        dense_size_++;

        return nextSparseIdx;
    }

    /*
     * Returns true if set contains element associated with the given index
     */
    inline bool contains(uint32_t index)
    {
        return index < sparse_size_ && sparse_[index] != null_index();
    }

    /*
     * Returns element if set contains it associated with given index. Throws runtime_error if index doesn't exist
     */
    T& get(uint32_t index)
    {
        if (!contains(index))
        {
            throw_no_index(index);
        }

        return dense_[sparse_[index]];
    }

    void remove(uint32_t index)
    {
        if (!contains(index))
        {
            throw_no_index(index);
        }

        sparse_[index] = null_index();

        if (index == sparse_size_ - 1)
        {
            sparse_size_--;
        }
        dense_size_--;
    }

    /*
     * Reserve greater capacity, than exist. Note, that it throw exception if capacity equals or lesser than current
     */
    void reserve(uint32_t newCapacity)
    {
        if (newCapacity <= capacity_)
        {
            throw std::runtime_error { "Can't reserve a new capacity lesser or equal to capacity()" };
        }

        uint32_t* newSparse = sparseAlloc_.allocate(newCapacity);
        T* newDense = denseAlloc_.allocate(newCapacity);

        copy_data(sparse_, dense_, newSparse, newDense, sparse_size_, dense_size_);

        sparseAlloc_.deallocate(sparse_, capacity_);
        denseAlloc_.deallocate(dense_, capacity_);

        sparse_ = newSparse;
        dense_ = newDense;
        capacity_ = newCapacity;
    }

    inline uint32_t capacity() const { return capacity_; }
    inline uint32_t size() const { return dense_size_; }
    inline uint32_t indices_size() const { return sparse_size_; }
    inline bool empty() const { return dense_size_ == 0; }
    inline const uint32_t* indices_data() const { return sparse_; }
    inline const T* data() const { return dense_; }
    static constexpr inline uint32_t null_index() { return NULL_INDEX; }
    static constexpr inline uint32_t max_index() { return NULL_INDEX - 1; }

private:
    static inline void copy(const SparseSet& from, SparseSet& to)
    {
        if (to.capacity_ > 0)
        {
            to.sparseAlloc_.deallocate(to.sparse_, to.capacity_);
            to.denseAlloc_.deallocate(to.dense_, to.capacity_);
        }

        to.sparseAlloc_ = from.sparseAlloc_;
        to.denseAlloc_ = from.denseAlloc_;
        to.sparse_size_ = from.sparse_size_;
        to.dense_size_ = from.dense_size_;
        to.capacity_ = from.capacity_;
        to.sparse_ = to.sparseAlloc_.allocate(from.capacity_);
        to.dense_ = to.denseAlloc_.allocate(from.capacity_);

        copy_data(
            from.sparse_, from.dense_,
            to.sparse_, to.dense_,
            from.sparse_size_, from.dense_size_);
    }

    static inline void copy_data(
        const uint32_t* sparseFrom, const T* denseFrom,
        uint32_t* sparseTo, T* denseTo, uint32_t sparseSize, uint32_t denseSize)
    {
        std::copy(sparseFrom, sparseFrom + sparseSize, sparseTo);
        std::copy(denseFrom, denseFrom + denseSize, denseTo);
    }

    static inline void throw_no_index(uint32_t index)
    {
        throw std::runtime_error{"There is no elements by a given index: " + std::to_string(index)};
    }

private:
    sparse_allocator sparseAlloc_;
    dense_allocator denseAlloc_;

    uint32_t* sparse_;
    T* dense_;
    uint32_t sparse_size_;
    uint32_t dense_size_;
    uint32_t capacity_;
};