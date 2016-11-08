burst
=====

A C++ template library for FPGAs on top of Xilinx Vivado HLS

> **Note that the current version of burst is an early preview. At this stage, the framework, including the API, are likely to undergo frequent changes.**

This library is currently at a proof of concept stage. It will (hopefully, and soon) help you to port existing C++ code that relies on templates and dynamic memory.

Why "burst"?
------------

Because of "burst reads" from AXI-Master interfaces, and because it differs from "boost" by only two characters.

Use burst like this:
--------------------

```C++
#include <algorithm>
#include <utility>

#include <burst/memory.h>

int test(volatile uint8_t* a, size_t n)
{
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS
#pragma HLS INTERFACE m_axi depth=256 port=a offset=slave bundle=MASTER_BUS

    auto mem = burst::memory(a, n);
    auto a_begin = mem.allocate<int>(8);
    auto a_end = a_begin + 8;

    std::fill(a_begin, a_end, 24);

    int arr[] = { 5, 2, 4, 9, 1, -1, 0, 12 };
    std::copy(arr, arr + 8, a_begin);

    std::make_heap(a_begin, a_end);
    std::sort_heap(a_begin, a_end);
    std::swap(a_begin[0], a_begin[1]);
    std::fill(a_begin + 2, a_begin + 6, 23);

    mem.deallocate(a_begin);

    return 0;
}
```

License
-------

Burst is licensed under the MIT License (MIT)
