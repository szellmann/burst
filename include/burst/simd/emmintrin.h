// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cassert>
#include <cstdint>

#include "detail/vec_types.h"


//-------------------------------------------------------------------------------------------------
// Types
//

typedef detail::int32x2_t __m64;
typedef detail::int64x2_t __m128i;


//-------------------------------------------------------------------------------------------------
// Load / store
//

FORCEINLINE __m128i _mm_load_si128(__m128i const* mem_addr)
{
    return *mem_addr;
}

FORCEINLINE void _mm_store_si128(__m128i* mem_addr, __m128i const& a)
{
    mem_addr[0] = a;
}


//-------------------------------------------------------------------------------------------------
// Set
//

FORCEINLINE __m128i _mm_set1_epi8(int8_t a)
{
    detail::int8x16_t tmp;
    for (int i = 0; i < 16; ++i)
    {
        tmp.value[i] = a;
    }
    return detail::convert<__m128i>(tmp);
}

FORCEINLINE __m128i _mm_set1_epi16(int16_t a)
{
    detail::int16x8_t tmp;
    for (int i = 0; i < 8; ++i)
    {
        tmp.value[i] = a;
    }
    return detail::convert<__m128i>(tmp);
}

FORCEINLINE __m128i _mm_set1_epi32(int32_t a)
{
    detail::int32x4_t tmp;
    for (int i = 0; i < 4; ++i)
    {
        tmp.value[i] = a;
    }
    return detail::convert<__m128i>(tmp);
}

//FORCEINLINE __m128i _mm_set1_epi64(__m64 const& a)
//{
//    detail::int64x2_t tmp;
//    for (int i = 0; i < 2; ++i)
//    {
//        tmp.value[i] = a;
//    }
//    return detail::convert<__m128i>(tmp);
//}

FORCEINLINE __m128i _mm_set_epi8(
        int8_t a16, int8_t a15, int8_t a14, int8_t a13,
        int8_t a12, int8_t a11, int8_t a10, int8_t  a9,
        int8_t  a8, int8_t  a7, int8_t  a6, int8_t  a5,
        int8_t  a4, int8_t  a3, int8_t  a2, int8_t  a1
        )
{
    detail::int8x16_t tmp = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 };
    return detail::convert<__m128i>(tmp);
}

FORCEINLINE __m128i _mm_set_epi16(
        int16_t a8, int16_t a7, int16_t a6, int16_t a5,
        int16_t a4, int16_t a3, int16_t a2, int16_t a1
        )
{
    detail::int16x8_t tmp = { a1, a2, a3, a4, a5, a6, a7, a8 };
    return detail::convert<__m128i>(tmp);
}

FORCEINLINE __m128i _mm_set_epi32(int32_t a4, int32_t a3, int32_t a2, int32_t a1)
{
    detail::int32x4_t tmp = { a1, a2, a3, a4 };
    return detail::convert<__m128i>(tmp);
}

//FORCEINLINE __m128i _mm_set_epi64(__m64 const& a2, __m64 const& a1)
//{
//    detail::int64x2_t tmp = { a1, a2 };
//    return detail::convert<__m128i>(tmp);
//}


//-------------------------------------------------------------------------------------------------
// Basic arithmetic
//

FORCEINLINE __m128i _mm_add_epi8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    auto b8 = detail::convert<detail::int8x16_t>(b);
    detail::int8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = a8.value[i] + b8.value[i];
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_add_epi16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::int16x8_t>(a);
    auto b16 = detail::convert<detail::int16x8_t>(b);
    detail::int16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = a16.value[i] + b16.value[i];
    }

    return detail::convert<__m128i>(c16);
}

FORCEINLINE __m128i _mm_add_epi32(__m128i const& a, __m128i const& b)
{
    auto a32 = detail::convert<detail::int32x4_t>(a);
    auto b32 = detail::convert<detail::int32x4_t>(b);
    detail::int32x4_t c32;

    for (int i = 0; i < 4; ++i)
    {
        c32.value[i] = a32.value[i] + b32.value[i];
    }

    return detail::convert<__m128i>(c32);
}

//FORCEINLINE __m128i _mm_add_epi64(__m128i const& a, __m128i const& b)
//{
//    auto a64 = detail::convert<detail::int64x2_t>(a);
//    auto b64 = detail::convert<detail::int64x2_t>(b);
//    detail::int64x2_t c64;
//
//    for (int i = 0; i < 2; ++i)
//    {
//        c64.value[i] = a64.value[i] + b64.value[i];
//    }
//
//    return detail::convert<__m128i>(c64);
//}


//-------------------------------------------------------------------------------------------------
// Saturated add/sub
//

FORCEINLINE __m128i _mm_adds_epi8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    auto b8 = detail::convert<detail::int8x16_t>(b);
    detail::int8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = detail::signed_saturated_add(a8.value[i], b8.value[i]);
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_adds_epi16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::int16x8_t>(a);
    auto b16 = detail::convert<detail::int16x8_t>(b);
    detail::int16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = detail::signed_saturated_add(a16.value[i], b16.value[i]);
    }

    return detail::convert<__m128i>(c16);
}

FORCEINLINE __m128i _mm_adds_epi32(__m128i const& a, __m128i const& b)
{
    auto a32 = detail::convert<detail::int32x4_t>(a);
    auto b32 = detail::convert<detail::int32x4_t>(b);
    detail::int32x4_t c32;

    for (int i = 0; i < 4; ++i)
    {
        c32.value[i] = detail::signed_saturated_add(a32.value[i], b32.value[i]);
    }

    return detail::convert<__m128i>(c32);
}

FORCEINLINE __m128i _mm_adds_epu8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::uint8x16_t>(a);
    auto b8 = detail::convert<detail::uint8x16_t>(b);
    detail::uint8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = detail::unsigned_saturated_add(a8.value[i], b8.value[i]);
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_adds_epu16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::uint16x8_t>(a);
    auto b16 = detail::convert<detail::uint16x8_t>(b);
    detail::uint16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = detail::unsigned_saturated_add(a16.value[i], b16.value[i]);
    }

    return detail::convert<__m128i>(c16);
}

FORCEINLINE __m128i _mm_subs_epi8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    auto b8 = detail::convert<detail::int8x16_t>(b);
    detail::int8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = detail::signed_saturated_sub(a8.value[i], b8.value[i]);
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_subs_epi16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::int16x8_t>(a);
    auto b16 = detail::convert<detail::int16x8_t>(b);
    detail::int16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = detail::signed_saturated_sub(a16.value[i], b16.value[i]);
    }

    return detail::convert<__m128i>(c16);
}

FORCEINLINE __m128i _mm_subs_epi32(__m128i const& a, __m128i const& b)
{
    auto a32 = detail::convert<detail::int32x4_t>(a);
    auto b32 = detail::convert<detail::int32x4_t>(b);
    detail::int32x4_t c32;

    for (int i = 0; i < 4; ++i)
    {
        c32.value[i] = detail::signed_saturated_sub(a32.value[i], b32.value[i]);
    }

    return detail::convert<__m128i>(c32);
}

FORCEINLINE __m128i _mm_subs_epu8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::uint8x16_t>(a);
    auto b8 = detail::convert<detail::uint8x16_t>(b);
    detail::uint8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = detail::unsigned_saturated_sub(a8.value[i], b8.value[i]);
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_subs_epu16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::uint16x8_t>(a);
    auto b16 = detail::convert<detail::uint16x8_t>(b);
    detail::uint16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = detail::unsigned_saturated_sub(a16.value[i], b16.value[i]);
    }

    return detail::convert<__m128i>(c16);
}


//-------------------------------------------------------------------------------------------------
// Bitwise
//

FORCEINLINE __m128i _mm_slli_si128(__m128i const& a, int imm)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    detail::int8x16_t result;

    for (int i = 0; i < 16; ++i)
    {
        result.value[i] = i - imm >= 0 ? a8.value[i - imm] : 0;
    }

    return detail::convert<__m128i>(result);
}

FORCEINLINE __m128i _mm_srli_si128(__m128i const& a, int imm)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    detail::int8x16_t result;

    for (int i = 0; i < 16; ++i)
    {
        result.value[i] = i + imm < 16 ? a8.value[i + imm] : 0;
    }

    return detail::convert<__m128i>(result);
}


//-------------------------------------------------------------------------------------------------
// Comparisons
//

FORCEINLINE __m128i _mm_cmpeq_epi8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    auto b8 = detail::convert<detail::int8x16_t>(b);
    detail::int8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = a8.value[i] == b8.value[i] ? 0xFF : 0x0;
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_cmpeq_epi16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::int16x8_t>(a);
    auto b16 = detail::convert<detail::int16x8_t>(b);
    detail::int16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = a16.value[i] == b16.value[i] ? 0xFFFF : 0x0;
    }

    return detail::convert<__m128i>(c16);
}

FORCEINLINE __m128i _mm_cmpgt_epi8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    auto b8 = detail::convert<detail::int8x16_t>(b);
    detail::int8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = a8.value[i] > b8.value[i] ? 0xFF : 0x0;
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_cmpgt_epi16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::int16x8_t>(a);
    auto b16 = detail::convert<detail::int16x8_t>(b);
    detail::int16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = a16.value[i] > b16.value[i] ? 0xFFFF : 0x0;
    }

    return detail::convert<__m128i>(c16);
}


//-------------------------------------------------------------------------------------------------
// Compact
//

int _mm_extract_epi16(__m128i const& a, int imm)
{
    assert(imm >= 0 && imm < 8);
    auto a16 = detail::convert<detail::int16x8_t>(a);
    return a16.value[imm];
}

int _mm_movemask_epi8(__m128i const& a)
{
    auto a8 = detail::convert<detail::int8x16_t>(a);
    return
        (a8.value[15] >> 7) << 15 |
        (a8.value[14] >> 7) << 14 |
        (a8.value[13] >> 7) << 13 |
        (a8.value[12] >> 7) << 12 |
        (a8.value[11] >> 7) << 11 |
        (a8.value[10] >> 7) << 10 |
        (a8.value[ 9] >> 7) <<  9 |
        (a8.value[ 8] >> 7) <<  8 |
        (a8.value[ 7] >> 7) <<  7 |
        (a8.value[ 6] >> 7) <<  6 |
        (a8.value[ 5] >> 7) <<  5 |
        (a8.value[ 4] >> 7) <<  4 |
        (a8.value[ 3] >> 7) <<  3 |
        (a8.value[ 2] >> 7) <<  2 |
        (a8.value[ 1] >> 7) <<  1 |
        (a8.value[ 0] >> 7);
}


//-------------------------------------------------------------------------------------------------
// stdlib-like
//

FORCEINLINE __m128i _mm_max_epu8(__m128i const& a, __m128i const& b)
{
    auto a8 = detail::convert<detail::uint8x16_t>(a);
    auto b8 = detail::convert<detail::uint8x16_t>(b);
    detail::uint8x16_t c8;

    for (int i = 0; i < 16; ++i)
    {
        c8.value[i] = a8.value[i] < b8.value[i]
            ? b8.value[i]
            : a8.value[i]
            ;
    }

    return detail::convert<__m128i>(c8);
}

FORCEINLINE __m128i _mm_max_epi16(__m128i const& a, __m128i const& b)
{
    auto a16 = detail::convert<detail::int16x8_t>(a);
    auto b16 = detail::convert<detail::int16x8_t>(b);
    detail::int16x8_t c16;

    for (int i = 0; i < 8; ++i)
    {
        c16.value[i] = a16.value[i] < b16.value[i]
            ? b16.value[i]
            : a16.value[i]
            ;
    }

    return detail::convert<__m128i>(c16);
}
