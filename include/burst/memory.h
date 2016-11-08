// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cstddef>
#include <cstdint>

#include "config.h"
#include "rand_iterator.h"

namespace burst
{

class memory
{
public:

    typedef config::size_type       size_type;
    typedef config::difference_type difference_type;

public:

    memory(volatile uint8_t* a, size_type n);

    template <typename T>
    rand_iterator<T> allocate(size_type n, void* /*hint*/ = 0);

    template <typename T>
    void deallocate(rand_iterator<T> ptr);

    volatile uint8_t* data;
    volatile size_type N;
};

} // namespace burst

#include "detail/memory.inl"
