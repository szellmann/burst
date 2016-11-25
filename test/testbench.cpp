// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <cstddef>
#include <cstring> // memset
#include <iostream>
#include <ostream>

extern int test1(volatile uint8_t* a, size_t n);

int main()
{
	int a[256];
	memset(a, 0, sizeof(a));
	size_t n = (sizeof(a) / sizeof(int));

	int result = test1(reinterpret_cast<volatile uint8_t*>(a), n);

	for (auto i : a)
		std::cout << i << ' ';
	std::cout << '\n';

	return result;
}
