// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <algorithm>
#include <utility>

#include <burst/memory.h>

int test(volatile uint8_t* a, size_t n)
{
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS
#pragma HLS INTERFACE m_axi depth=256 port=a offset=slave bundle=MASTER_BUS

	auto reg = burst::memory::region(a, n);
	auto a_begin = reg.allocate<int>(8);
	auto a_end = a_begin + 8;

	std::fill(a_begin, a_end, 24);

	int arr[] = { 5, 2, 4, 9, 1, -1, 0, 12 };
	std::copy(arr, arr + 8, a_begin);

	std::make_heap(a_begin, a_end);
	std::sort_heap(a_begin, a_end);
	std::swap(a_begin[0], a_begin[1]);
	std::fill(a_begin + 2, a_begin + 6, 23);
	std::rotate(a_begin, a_begin + 2, a_end);

	reg.deallocate(a_begin);

	return 0;
}
