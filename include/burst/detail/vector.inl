// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <algorithm>
#include <iterator>

namespace burst
{

template <typename T, typename Alloc>
inline vector<T, Alloc>::vector()
    : size_(0)
    , capacity_(0)
{
}

template <typename T, typename Alloc>
inline vector<T, Alloc>::vector(typename vector<T, Alloc>::size_type count)
    : size_(count)
    , capacity_(0)
{
    grow_by(size_);
}

template <typename T, typename Alloc>
inline vector<T, Alloc>::vector(std::initializer_list<T> init, Alloc const&)
    : size_(0)
    , capacity_(0)
{
    grow_by(init.size());
    size_ = init.size();
    std::copy(init.begin(), init.end(), first_);
}

template <typename T, typename Alloc>
inline vector<T, Alloc>::vector(
        typename vector<T, Alloc>::const_iterator first,
        typename vector<T, Alloc>::const_iterator last
        )
    : size_(std::distance(first, last))
    , capacity_(0)
{
    grow_by(size_);
    std::copy(first, last, first_);
}

// Iterators ----------------------------------------------

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::begin()
{
    return first_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::begin() const
{
    return first_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const
{
    return first_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::end()
{
    return first_ + size_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::end() const
{
    return first_ + size_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const
{
    return first_ + size_;
}

// Element access -----------------------------------------

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::at(
        typename vector<T, Alloc>::size_type pos
        )
{
    // TODO: emulate "throw std::out_of_range"
    return operator[](pos);
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::at(
        typename vector<T, Alloc>::size_type pos
        ) const
{
    // TODO: emulate "throw std::out_of_range"
    return operator[](pos);
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](
        typename vector<T, Alloc>::size_type pos
        )
{
    return *(first_ + pos);
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](
        typename vector<T, Alloc>::size_type pos
        ) const
{
    return *(first_ + pos);
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::front()
{
    return *first_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const
{
    return *first_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::reference vector<T, Alloc>::back()
{
    return *(first_ + size_ - 1);
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::const_reference vector<T, Alloc>::back() const
{
    return *(first_ + size_ - 1);
}

// Capacity -----------------------------------------------

template <typename T, typename Alloc>
inline bool vector<T, Alloc>::empty() const
{
    return size_ == 0;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const
{
    return size_;
}

template <typename T, typename Alloc>
inline typename vector<T, Alloc>::size_type vector<T, Alloc>::max_size() const
{
    return size_;
}

template <typename T, typename Alloc>
inline void vector<T, Alloc>::reserve(typename vector<T, Alloc>::size_type new_cap)
{
}

// Modifiers ----------------------------------------------

template <typename T, typename Alloc>
inline void vector<T, Alloc>::push_back(T const& value)
{
    if (capacity_ < size_ + 1)
    {
        grow_by(1);
    }

    *(first_ + size_) = value;
    ++size_;
}

// private ------------------------------------------------

template <typename T, typename Alloc>
inline void vector<T, Alloc>::grow_by(typename vector<T, Alloc>::size_type s)
{
    Alloc alloc;

    if (capacity_ == 0)
    {
        first_ = alloc.allocate(s);
    }
    else
    {
        auto tmp = alloc.allocate(s);
        std::copy(first_, first_ + size_, tmp);
        alloc.deallocate(first_, capacity_);
        first_ = tmp;
    }

    capacity_ += s;
}

template <typename T, typename Alloc>
inline void vector<T, Alloc>::shrink_by(typename vector<T, Alloc>::size_type s)
{
    Alloc alloc;
    first_ = alloc.allocate(s);
}

} // namespace burst
