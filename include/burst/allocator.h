// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include "memory.h"
#include "rand_iterator.h"

namespace burst
{

template <typename T, memory::region_id Id>
class allocator
{
public:

    typedef T                                           value_type;
    typedef rand_iterator<T>                            pointer;
    typedef const rand_iterator<T>                      const_pointer;
    typedef typename rand_iterator<T>::reference        reference;
    typedef typename rand_iterator<T>::const_reference  const_reference;
    typedef typename rand_iterator<T>::size_type        size_type;
    typedef typename rand_iterator<T>::difference_type  difference_type;

public:

    allocator() = default;

    template <typename U>
    allocator(allocator<U, Id> const& /*rhs*/)
    {
        //TODO
    }

    template <typename U>
    struct rebind
    {
        typedef allocator<U, Id> other;
    };

    pointer address(reference r) const
    {
        return &r;
    }

    const_pointer address(const_reference r) const
    {
    	return &r;
    }

    pointer allocate(size_type n, void* /*hint*/ = 0)
    {

    	return memory::allocate<T>(n, Id);
    }

    void deallocate(pointer p, size_type /*n*/)
    {
    	memory::deallocate(p, Id);
    }

    size_t max_size() const
    {
    	return memory::default_regions[Id].N;
    }

    void construct(pointer p, const_reference val)
    {

    }

    void destroy(pointer p)
    {

    }

    bool operator==(allocator const& rhs) const
    {
        return true;
    }

    bool operator!=(allocator const& rhs) const
    {
        return !(*this == rhs);
    }

private:

};

} // namespace burst

#include "detail/allocator.inl"
