// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <cassert>

#include <burst/memory.h>

namespace burst
{
namespace memory
{

region default_regions[RegionMax];

//-------------------------------------------------------------------------------------------------
// Initialize a default memory region
//

void init(volatile uint8_t* a, region::size_type n, region_id id)
{
    assert(a != nullptr);
    assert(id < RegionMax);

    default_regions[id] = region(a, n);
}

}
}
