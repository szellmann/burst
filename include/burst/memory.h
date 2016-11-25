// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cstddef>
#include <cstdint>

#include "config.h"
#include "rand_iterator.h"

namespace burst
{

struct memory
{
    memory(volatile uint8_t* data, config::size_type N);

    template <typename T>
    rand_iterator<T> allocate(config::size_type n);

    template <typename T>
    void deallocate(rand_iterator<T> ptr);

    volatile uint8_t* data;
    volatile config::size_type N;
};

} // namespace burst

#include "detail/memory.inl"
