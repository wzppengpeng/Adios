#ifndef WZP_CONTAINER_ARRAY_VIEW_HPP_
#define WZP_CONTAINER_ARRAY_VIEW_HPP_

#include <vector>
#include <array>

/**
 * \brief Read only data structure to reference continuous memory region of array.
 * Provide unified view for vector, array and C style array.
 * This data structure do not guarantee aliveness of referenced array.
 *
 * Make sure do not use array_view to record data in async function closures.
 * Also do not use array_view to create reference to temporary data structure.
 *
 * \tparam ValueType The value
 *
 * \code
 *  std::vector<int> myvec{1,2,3};
 *  wzp::array_view<int> view(myvec);
 *  // indexed visit to the view.
 *  cout << view[0];
 *
 *  for (int v : view) {
 *     // visit each element in the view
 *  }
 *
 * this class is for some function to support the fixed interface
 */

namespace wzp {

template<typename ValueType>
class array_view {
public:
    // the default constructor
    array_view() = default;
    // copy constructor
    array_view(const array_view<ValueType>& other) = default;
    // move constructor
    array_view(array_view<ValueType>&& other) = default;
    // assign
    array_view<ValueType>& operator= (const array_view<ValueType>& other) = default;

    // construct by std::vector
    array_view(const std::vector<ValueType>& other) {
        if(!other.empty()) {
            begin_ptr_ = other.data();
            size_ = other.size();
        }
    }

    // constrcut by std::array
    template<size_t size>
    array_view(const std::array<ValueType, size>& other) {
        if(!other.empty()) {
            begin_ptr_ = other.data();
            size_ = other.size();
        }
    }

    // construct by two pointer
    array_view(const ValueType* begin, const ValueType* end) {
        if(begin < end) {
            begin_ptr_ = begin;
            size_ = end - begin;
        }
    }

    // construct by pointer with size
    array_view(const ValueType* begin, size_t length) {
        if(length > 0) {
            begin_ptr_ = begin;
            size_ = length;
        }
    }

    /*! \return if it is empty*/
    inline bool empty() const {
        return size_ == 0;
    }

    /*! \return size of the array */
    inline size_t size() const {
        return size_;
    }
    /*! \return begin of the array */
    inline const ValueType* begin() const {
        return begin_ptr_;
    }
    /*! \return end point of the array */
    inline const ValueType* end() const {
        return begin_ptr_ + size_;
    }
    /*!
     * \brief get i-th element from the view
     * \param i The index.
     * \return const reference to i-th element.
     */
    inline const ValueType& operator[](size_t i) const {
        return begin_ptr_[i];
    }

private:
    // the pointer to first data
    const ValueType* begin_ptr_ {nullptr};
    // the array size
    size_t size_ {0};
};

}

#endif /*WZP_CONTAINER_ARRAY_VIEW_HPP_*/