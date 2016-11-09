// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include "memory.h"
#include "rand_iterator.h"

namespace burst
{

template <typename T>
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

    allocator(memory::region mem)
        : mem_(mem)
    {
    }

    template <typename U>
    allocator(allocator<U> const& /*rhs*/)
    {
        //TODO
    }

    template <typename U>
    struct rebind
    {
        typedef allocator<U> other;
    };

    pointer address(reference r) const
    {
        return &r;
    }

private:

    memory mem_;

};

} // namespace burst

#include "detail/allocator.inl"
