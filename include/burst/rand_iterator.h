// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <utility>
#include <iostream>

#include "config.h"


namespace burst
{
namespace detail
{
namespace randit
{

template <typename T>
struct reference
{
    typedef config::difference_type difference_type;
    typedef T value_type;

    T value;
    difference_type index;
    volatile uint8_t* raw;


    reference()
        : value(0)
        , index(0)
        , raw(0)
    {

    }

    reference(reference const& rhs)
        : value(rhs.value)
        , index(rhs.index)
        , raw(rhs.raw)
    {

    }

    reference(reference const&& rhs)
        : value(std::move(rhs.value))
        , index(std::move(rhs.index))
        , raw(std::move(rhs.raw))
    {

    }

    ~reference()
    {

    }

    void reset(T const& val)
    {
        value = val;

        config::size_type stride = sizeof(T);

//            std::memcpy(
//                    (uint8_t*)(&raw[index * stride]),
//                    &value,
//                    sizeof(T)
//                    );
        for (int i = 0; i < stride; ++i)
        {
            //#pragma HLS PIPELINE
            uint8_t tmp = (value >> (i * 8)) & 0xFF;

            raw[index * stride + i] = tmp;
        }
    }

    reference& operator=(reference const& rhs)
    {
        if (&rhs != this)
        {
            reset(rhs.value);
            index = rhs.index;
            raw   = rhs.raw;
        }

        return *this;
    }

    reference& operator=(reference const&& rhs)
    {
        if (&rhs != this)
        {
            reset(std::move(rhs.value));
            index = std::move(rhs.index);
            raw   = std::move(rhs.raw);
        }

        return *this;
    }

    operator T const&() const
    {
        return value;
    }

    operator T&()
    {
        return value;
    }

    void swap(reference rhs)
    {
        T value_l = value;
        T value_r = rhs.value;

        reset(value_r);
        rhs.reset(value_l);
    }

    // Manipulate value -----------------------------------

    reference& operator=(T const& val)
    {
        reset(val);
        return *this;
    }

    reference& operator+=(T const& val)
    {
        reset(value + val);
        return *this;
    }

    reference& operator-=(T const& val)
    {
        reset(value - val);
        return *this;
    }

    reference& operator*=(T const& val)
    {
        reset(value * val);
        return *this;
    }

    reference& operator/=(T const& val)
    {
        reset(value / val);
        return *this;
    }

    reference& operator%=(T const& val)
    {
        reset(value % val);
        return *this;
    }

    reference& operator<<=(T const& val)
    {
        reset(value << val);
        return *this;
    }

    reference& operator>>=(T const& val)
    {
        reset(value >> val);
        return *this;
    }

    reference& operator&=(T const& val)
    {
        reset(value & val);
        return *this;
    }

    reference& operator^=(T const& val)
    {
        reset(value ^ val);
        return *this;
    }

    reference& operator|=(T const& val)
    {
        reset(value | val);
        return *this;
    }
};

template <typename T>
struct const_reference
{
    typedef config::difference_type difference_type;
    typedef T value_type;

    T value;
    difference_type index;
    volatile uint8_t* raw;

    const_reference()
        : value(0)
        , index(0)
        , raw(0)
    {

    }

    operator T const&() const
    {
        return value;
    }
};

template <typename T>
inline void swap(reference<T> a, reference<T> b)
{
    a.swap(b);
}

} // namespace randit
} // namespace detail


template <typename T>
class rand_iterator : public std::iterator<std::random_access_iterator_tag, T>
{
    static_assert( sizeof(T) >= sizeof(uint8_t), "Size mismatch");

public:
    typedef config::size_type size_type;
    typedef config::difference_type difference_type;

    typedef typename detail::randit::reference<T> reference;
    typedef typename detail::randit::const_reference<T> const_reference;

    template <typename>
    friend rand_iterator operator+(rand_iterator const& a, difference_type i);

public:
    rand_iterator()
        : raw_(0)
        , pos_(0)
    {
    }

    explicit rand_iterator(volatile uint8_t* raw)
        : raw_(raw)
        , pos_(0)
    {
    }

    rand_iterator(volatile uint8_t* raw, difference_type pos)
        : raw_(raw)
        , pos_(pos)
    {
    }

    rand_iterator(rand_iterator const& rhs)
        : raw_(rhs.raw_)
        , pos_(rhs.pos_)
    {
    }

    ~rand_iterator()
    {
    }

    rand_iterator& operator=(rand_iterator const& rhs)
    {
        if (&rhs != this)
        {
            raw_ = rhs.raw_;
            pos_ = rhs.pos_;
        }

        return *this;
    }

    reference operator[](difference_type n)
    {
        reference result;

        config::size_type stride = sizeof(T);

        result.raw = raw_;
        result.index = pos_ + n;

//        std::memcpy(
//                &result.value,
//                (uint8_t const*)(&raw_[n * stride]),
//                sizeof(T)
//                );
        for (int i = 0; i < stride; ++i)
        {
            //#pragma HLS PIPELINE
            T val = raw_[(pos_ + n) * stride + i] << (i * 8);
            result.value += val;
        }

        return result;
    }

    const_reference operator[](difference_type n) const
    {
        reference result;

        config::size_type stride = sizeof(T);

        result.index = pos_ + n;
        result.raw = raw_;

//        std::memcpy(
//                &result.value,
//                (uint8_t const*)(&raw_[n * stride]),
//                sizeof(T)
//                );
        for (int i = 0; i < stride; ++i)
        {
            //#pragma HLS PIPELINE
            T val = raw_[(pos_ + n) * stride + i] << (i * 8);
            result.value += val;
        }

        return result;
    }

    reference operator*()
    {
        return operator[](0);
    }

    const_reference operator*() const
    {
        return operator[](0);
    }

    rand_iterator& operator++()
    {
        pos_ += 1;
        return *this;
    }

    rand_iterator& operator--()
    {
        pos_ -= 1;
        return *this;
    }

    rand_iterator operator++(int)
    {
        rand_iterator old = *this;
        this->operator++();
        return old;
    }

    volatile difference_type& pos()
    {
        return pos_;
    }

    volatile difference_type const& pos() const
    {
        return pos_;
    }

    volatile uint8_t* data()
    {
        return raw_;
    }

    volatile uint8_t const* data() const
    {
        return raw_;
    }

    void swap(rand_iterator rhs)
    {
        (*this).swap(*rhs);
    }

private:
    volatile uint8_t* raw_;
    volatile difference_type pos_;

};


template <typename T>
bool operator==(rand_iterator<T> a, rand_iterator<T> b)
{
    //TODO: store some hash to avoid pointer comparison
    return /*a.data() == b.data() &&*/ a.pos() == b.pos();
}

template <typename T>
bool operator!=(rand_iterator<T> a, rand_iterator<T> b)
{
    //TODO: store some hash to avoid pointer comparison
    return /*a.data() != b.data() ||*/ a.pos() != b.pos();
}

template <typename T>
rand_iterator<T> operator+(rand_iterator<T> const& a, typename rand_iterator<T>::difference_type n)
{
    rand_iterator<T> result(a);
    result.pos() = a.pos() + n;
    return result;
}

template <typename T>
rand_iterator<T> operator-(rand_iterator<T> const& a, typename rand_iterator<T>::difference_type n)
{
    rand_iterator<T> result(a);
    result.pos() = a.pos() - n;
    return result;
}

template <typename T>
typename rand_iterator<T>::difference_type operator-(rand_iterator<T> a, rand_iterator<T> b)
{
    return a.pos() - b.pos();
}

template <typename T>
rand_iterator<T>& operator+=(rand_iterator<T>& ptr, typename rand_iterator<T>::difference_type n)
{
    ptr = ptr + n;
    return ptr;
}

//template <typename T>
//static_pointer<T>& operator-=(
//      static_pointer<T>& ptr,
//      typename static_pointer<T>::difference_type n
//      )
//{
//    ptr = ptr - n;
//    return ptr;
//}

} // namespace burst


namespace std
{

template <typename T>
struct iterator_traits<burst::rand_iterator<T> >
{
    typedef T value_type;
    typedef typename burst::rand_iterator<T>::iterator_category iterator_category;
    typedef typename burst::rand_iterator<T>::reference reference;
    typedef typename burst::rand_iterator<T>::const_reference const_reference;
    typedef typename burst::rand_iterator<T>::difference_type difference_type;
};


template <typename T>
void swap(burst::detail::randit::reference<T> a, burst::detail::randit::reference<T> b)
{
    burst::detail::randit::swap(a, b);
}

} // namespace std
