// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#define FORCEINLINE inline
#define SIMD_LITTLE_ENDIAN 1

namespace detail
{

//-------------------------------------------------------------------------------------------------
// Signed types
//

struct int8x8_t
{
    enum { N = 8 };
    typedef int8_t value_type;
    int8_t value[8];
};

struct int8x16_t
{
    enum { N = 16 };
    typedef int8_t value_type;
    int8_t value[16];
};

struct int16x8_t
{
    enum { N = 8 };
    typedef int16_t value_type;
    int16_t value[8];
};

struct int32x2_t
{
    enum { N = 2 };
    typedef int32_t value_type;
    int32_t value[2];
};

struct int32x4_t
{
    enum { N = 4 };
    typedef int32_t value_type;
    int32_t value[4];
};

struct int64x2_t
{
    enum { N = 2 };
    typedef int64_t value_type;
    int64_t value[2];
};


//-------------------------------------------------------------------------------------------------
// Signed types
//

struct uint8x16_t
{
    enum { N = 16 };
    typedef uint8_t value_type;
    uint8_t value[16];
};

struct uint16x8_t
{
    enum { N = 8 };
    typedef uint16_t value_type;
    uint16_t value[8];
};

struct uint32x4_t
{
    enum { N = 4 };
    typedef uint32_t value_type;
    uint32_t value[4];
};

struct uint64x2_t
{
    enum { N = 2 };
    typedef uint64_t value_type;
    uint64_t value[2];
};


//-------------------------------------------------------------------------------------------------
//
//

template <typename T1>
struct swap_sign;

template <>
struct swap_sign<int8x16_t> { typedef uint8x16_t value_type; };

template <>
struct swap_sign<uint8x16_t> { typedef int8x16_t value_type; };

template <>
struct swap_sign<int16x8_t> { typedef uint16x8_t value_type; };

template <>
struct swap_sign<uint16x8_t> { typedef int16x8_t value_type; };

template <>
struct swap_sign<int32x4_t> { typedef uint32x4_t value_type; };

template <>
struct swap_sign<uint32x4_t> { typedef int32x4_t value_type; };

template <>
struct swap_sign<int64x2_t> { typedef uint64x2_t value_type; };

template <>
struct swap_sign<uint64x2_t> { typedef int64x2_t value_type; };


//-------------------------------------------------------------------------------------------------
// Conversion
//

template <typename I1, typename I2>
FORCEINLINE I1 convert_up(I2 const& a)
{
    auto stride = I2::N / I1::N;
    auto shift = sizeof(typename I2::value_type) * 8;

    I1 result;
    typedef typename std::make_unsigned<typename I2::value_type>::type UI2;
    for (int i = 0; i < I2::N; i += stride)
    {
        result.value[i / stride] = UI2(a.value[i + stride - 1]);
        for (int j = 1; j < stride; ++j)
        {
#if SIMD_LITTLE_ENDIAN
            result.value[i / stride] <<= shift;
#else
            result.value[i / stride] >>= shift;
#endif
            result.value[i / stride] |= UI2(a.value[i + stride - j - 1]);
        }
    }

    return result;
}

template <typename I1, typename I2>
FORCEINLINE I1 convert_down(I2 const& a)
{
    auto stride = I1::N/ I2::N;
    auto shift = sizeof(typename I1::value_type) * 8;

    I1 result;
    for (int i = 0; i < I1::N; i += stride)
    {
        auto val = a.value[i / stride];

        for (int j = 0; j < stride; ++j)
        {
            result.value[i + j] = typename I1::value_type(val);
#if SIMD_LITTLE_ENDIAN
            val >>= shift;
#else
            val <<= shift;
#endif
        }
    }
    return result;
}

template <typename T1, typename T2>
FORCEINLINE T1 convert_static(T2 const& a)
{
    static_assert(sizeof(T1) == sizeof(T2), "Size mismatch");
    static_assert(T1::N == T2::N, "Element count mismatch");

    T1 result;

    for (int i = 0; i < T1::N; ++i)
    {
        result.value[i] = static_cast<typename T1::value_type>(a.value[i]);
    }

    return result;
}

template <typename I>
FORCEINLINE I convert(I const& a)
{
    return a;
}

template <
    typename I1,
    typename I2,
    typename = typename std::enable_if<!std::is_same<I1, I2>::value>::type,
    typename = typename std::enable_if<(std::is_signed<typename I1::value_type>::value &&  std::is_signed<typename I2::value_type>::value)
                                   || (!std::is_signed<typename I1::value_type>::value && !std::is_signed<typename I2::value_type>::value)
        >::type
    >
FORCEINLINE I1 convert(I2 const& a)
{
    if (sizeof(typename I1::value_type) < sizeof(typename I2::value_type))
    {
        return convert_down<I1>(a);
    }
    else
    {
        return convert_up<I1>(a);
    }
}

template <
    typename T1,
    typename T2,
    typename = typename std::enable_if<(std::is_signed<typename T1::value_type>::value && !std::is_signed<typename T2::value_type>::value)
                                    || (!std::is_signed<typename T1::value_type>::value && std::is_signed<typename T2::value_type>::value)
        >::type
    >
FORCEINLINE T1 convert(T2 const& a)
{
    auto tmp = convert_static<typename swap_sign<T2>::value_type>(a);
    return convert<T1>(tmp);
}


//-------------------------------------------------------------------------------------------------
//
//

template <typename I>
I signed_saturated_add(I a, I b)
{
    I sum = a + b;
    if (a >= 0 && b >= 0)
        return (a > std::numeric_limits<I>::max() - b)
            ? std::numeric_limits<I>::max()
            : a + b
            ;
    else if (a < 0 && b < 0)
        return (a < std::numeric_limits<I>::min() - b)
            ? std::numeric_limits<I>::min()
            : a + b
            ;
    else
        return a + b;
}

template <typename UI>
UI unsigned_saturated_add(UI a, UI b)
{
    return (a > std::numeric_limits<UI>::max() - b)
        ? std::numeric_limits<UI>::max()
        : a + b
        ;
}

template <typename I>
I signed_saturated_sub(I a, I b)
{
    if (a < 0 && b > 0)
        return (a < std::numeric_limits<I>::min() + b)
            ? std::numeric_limits<I>::min()
            : a - b
            ;
    else if (a > 0 && b < 0)
        return (a > std::numeric_limits<I>::max() + b)
            ? std::numeric_limits<I>::max()
            : a - b
            ;
    else if (a == 0 && b == std::numeric_limits<I>::min())
        return std::numeric_limits<I>::max();
    else
        return a - b;
}

template <typename UI>
UI unsigned_saturated_sub(UI a, UI b)
{
    return b > a ? 0U : a - b;
}

} // detail
