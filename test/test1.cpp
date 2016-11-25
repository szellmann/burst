// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <algorithm>
#include <utility>

#include <burst/memory.h>

int test1(volatile uint8_t* a, size_t n)
{
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS
#pragma HLS INTERFACE m_axi depth=512 port=a offset=slave bundle=MASTER_BUS
#pragma HLS INTERFACE s_axilite port=n bundle=CTRL_BUS

    burst::memory mem(a, n);

    auto a_begin = mem.allocate<int>(8);
    auto a_end = a_begin + 8;

    std::fill(a_begin, a_end, 24);

    int arr[] = { 5, 2, 4, 9, 1, -1, 0, 12 };
    std::copy(arr, arr + 8, a_begin);

    std::make_heap(a_begin, a_end);
    std::sort_heap(a_begin, a_end);
    std::swap(a_begin[0], a_begin[1]);
    std::fill(a_begin + 2, a_begin + 6, 23);
    std::rotate(a_begin, a_begin + 2, a_end);

    auto b_begin = mem.allocate<int>(4);

    mem.deallocate(a_begin);
    mem.deallocate(b_begin);

    return 0;
}
