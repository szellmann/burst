// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cstddef>
#include <cstdint>

#include "config.h"
#include "rand_iterator.h"

namespace burst
{
namespace memory
{

//-------------------------------------------------------------------------------------------------
// Wrapper class for memory regions
//

class region
{
public:

    typedef config::size_type       size_type;
    typedef config::difference_type difference_type;

public:

    region();
    region(volatile uint8_t* a, size_type n);

    template <typename T>
    rand_iterator<T> allocate(size_type n);

    template <typename T>
    void deallocate(rand_iterator<T> ptr);

    bool valid() const;

    volatile uint8_t* data;
    volatile size_type N;
};


//-------------------------------------------------------------------------------------------------
// Default memory regions
//

enum region_id
{
	Region0,
	Region1,
	Region2,
	Region3,
	Region4,
	Region5,
	Region6,
	Region7,
	RegionMax
};

extern region default_regions[RegionMax];


//-------------------------------------------------------------------------------------------------
// Initialize a default memory region
//

void init(volatile uint8_t* a, region::size_type n, region_id id = Region0);


//-------------------------------------------------------------------------------------------------
// Allocate/deallocate on one of the default memory regions
//

template <typename T>
rand_iterator<T> allocate(region::size_type n, region_id id = Region0);

template <typename T>
void deallocate(rand_iterator<T> ptr, region_id id = Region0);

} // namespace memory
} // namespace burst

#include "detail/memory.inl"
