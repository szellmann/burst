// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <algorithm>
#include <utility>

#include <burst/memory.h>
#include <burst/vector.h>

int test2(volatile uint8_t* a, size_t n)
{
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS
#pragma HLS INTERFACE m_axi depth=256 port=a offset=slave bundle=MASTER_BUS
#pragma HLS INTERFACE s_axilite port=n bundle=CTRL_BUS

    burst::memory::init(a, n);

    burst::vector<int> v1({0,1,2,3,4,5,6,7});
    v1.at(1) = 2;
    v1[2] = v1[1] + 1;
    v1.front() = 54;
    v1.back() = 8;

    return 0;
}
