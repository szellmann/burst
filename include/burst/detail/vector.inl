// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <algorithm>

namespace burst
{

template <typename T, typename Alloc>
inline vector<T, Alloc>::vector(std::initializer_list<T> init, Alloc const&)
{
    Alloc alloc;
    first_ = alloc.allocate(init.size());
    std::copy(init.begin(), init.end(), first_);
}

} // namespace burst
