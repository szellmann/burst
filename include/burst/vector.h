// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <initializer_list>

#include "allocator.h"
#include "memory.h"
#include "rand_iterator.h"

namespace burst
{

template <typename T, typename Alloc = allocator<T, memory::Region0>>
class vector
{
public:
    typedef T value_type;

public:

    vector() = default;
    vector(std::initializer_list<T> init, Alloc const& alloc = Alloc());

private:

    rand_iterator<T> first_;
    rand_iterator<T> last_;

};

} // namespace burst

#include "detail/vector.inl"
