// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <initializer_list>

#include "allocator.h"
#include "config.h"
#include "memory.h"
#include "rand_iterator.h"

namespace burst
{

template <typename T, typename Alloc = allocator<T, memory::Region0>>
class vector
{
public:
    typedef T                                           value_type;
    typedef Alloc                                       allocator_type;
    typedef config::size_type                           size_type;
    typedef config::difference_type                     difference_type;
    typedef rand_iterator<T>                            pointer;
    typedef const rand_iterator<T>                      const_pointer;
    typedef typename rand_iterator<T>::reference        reference;
    typedef typename rand_iterator<T>::const_reference  const_reference;
    typedef rand_iterator<T>                            iterator;
    typedef const rand_iterator<T>                      const_iterator;

public:

    // Construction etc. ----------------------------------

    vector();
    explicit vector(size_type count);
    vector(std::initializer_list<T> init, Alloc const& alloc = Alloc());

    // Element access -------------------------------------

    reference       at(size_type pos);
    const_reference at(size_type pos) const;
    reference       operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;

    // Iterators ------------------------------------------

    iterator        begin();
    const_iterator  begin() const;
    const_iterator cbegin() const;
    iterator        end();
    const_iterator  end() const;
    const_iterator cend() const;

    // Capacity -------------------------------------------

    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    void reserve(size_type new_cap);
    size_type capacity() const;
    void shrink_to_fit();

    // Modifiers ------------------------------------------

    void push_back(T const& value);

private:

    rand_iterator<T> first_;
    size_type size_;
    size_type capacity_;

    void grow_by(size_type s);
    void shrink_by(size_type s);
};

} // namespace burst

#include "detail/vector.inl"
