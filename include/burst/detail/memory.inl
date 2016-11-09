// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <cstddef>
#include <cstdint>
#include <cstring> // memcpy

#ifndef NDEBUG
#include <iostream>
#include <ostream>
#endif

#ifndef __SYNTHESIS__
//#include <boost/thread/mutex.hpp>
//#include <boost/thread/locks.hpp>
#endif

#include "../rand_iterator.h"

namespace burst
{
namespace memory
{

//enum { N = 1024 };

#ifndef __SYNTHESIS__
//static boost::mutex mtx;
#endif

struct node;
typedef node* node_ptr;


// node ---------------------------------------------------

struct node
{
    region::size_type pos;
    region::size_type size;
    region::size_type pred_pos;
    region::size_type allocated;
};

inline void make_node(
        volatile uint8_t* mem,
        region::size_type pos,
        region::size_type size,
        region::size_type pred_pos,
        bool allocated
        )
{
#pragma HLS INLINE
    node n;
    n.pos = pos;
    n.size = size;
    n.pred_pos = pred_pos;
    n.allocated = allocated;
    {
#ifndef __SYNTHESIS__
//      boost::lock_guard<boost::mutex> l(mtx);
#endif
        memcpy((uint8_t*)mem + pos, &n, sizeof(n));
    }
}

// Order matters!
inline void merge_nodes(
        volatile uint8_t* mem,
        node const& n1,
        node const& n2,
        bool allocated
        )
{
#pragma HLS INLINE
    make_node(mem, n1.pos, n1.size + n2.size, n1.pred_pos, allocated);
}

inline node get_node(volatile uint8_t* mem, region::size_type pos)
{
#pragma HLS INLINE
    node n;
    {
#ifndef __SYNTHESIS__
//      boost::lock_guard<boost::mutex> l(mtx);
#endif
        memcpy(&n, (uint8_t*)mem + pos, sizeof(n));
    }
    return n;
}

inline node get_node_from_ptr(volatile void const* ptr)
{
#pragma HLS INLINE
    node n;
    {
#ifndef __SYNTHESIS__
//        boost::lock_guard<boost::mutex> l(mtx);
#endif
        memcpy(&n, (void const*)ptr, sizeof(n));
    }
    return n;
}

inline node prev_node(volatile uint8_t* mem, node const& n)
{
#pragma HLS INLINE
    return get_node(mem, n.pred_pos);
}

inline node next_node(volatile uint8_t* mem, node const& n)
{
#pragma HLS INLINE
	region::size_type addr = n.pos + n.size;
    return get_node(mem, addr);
}

inline region::size_type get_data_addr(node const& n)
{
#pragma HLS INLINE
    return n.pos + sizeof(n);
}

inline bool operator!=(node const& n1, node const& n2)
{
    return n1.pos       != n2.pos       ||
           n1.size      != n2.size      ||
           n1.pred_pos  != n2.pred_pos  ||
           n1.allocated != n2.allocated
           ;
}

#ifndef NDEBUG
inline std::ostream& operator<<(std::ostream& out, node n)
{
    out << '(' << n.pos
        << ',' << n.size
        << ',' << n.pred_pos
        << ',' << n.allocated
         << ')';
    return out;
}
#endif


// list ---------------------------------------------------

static bool free_list_initialized = false;

inline void free_list_init(volatile uint8_t* mem, region::size_type N)
{
#pragma HLS INLINE
    make_node(mem, 0, N, 0, false);
}


inline node free_list_insert_first(volatile uint8_t* mem, region::size_type size)
{
#pragma HLS INLINE
	region::size_type addr = 0;

    node n = get_node(mem, addr);

    while (n.size < size || n.allocated)
    {
        n = next_node(mem, n);
    }


    node n1;
    n1.pos = n.pos;
    n1.size = size;
    n1.pred_pos = n.pred_pos;
    n1.allocated = true;

    node n2;
    n2.pos = n.pos + size;
    n2.size = n.size - size;
    n2.pred_pos = n.pos;
    n2.allocated = n.allocated;

    make_node(mem, n1.pos, n1.size, n1.pred_pos, n1.allocated);
    make_node(mem, n2.pos, n2.size, n2.pred_pos, n2.allocated);

    return n1;
}

#ifndef NDEBUG
inline void free_list_print(volatile uint8_t* mem, region::size_type N)
{
#ifndef __SYNTHESIS__
    node n = get_node(mem, 0);
    std::cout << "n= get_node(0): "<<n<< std::endl;

    while (n.pos + n.size < N)
    {
        n = next_node(mem, n);
        std::cout <<"n=next_node(n): "<<n<< std::endl;
    }
#endif
}
#endif


// interface ----------------------------------------------

inline region::region(volatile uint8_t* a, region::size_type n)
    : data(a)
    , N(n)
{
}

template <typename T>
inline rand_iterator<T> region::allocate(region::size_type n)
{
    if (!free_list_initialized)
    {
        free_list_init(data, N);
        free_list_initialized = true;
    }
    
    // Bytes
    region::size_type size = n * sizeof(T);

    node nd = free_list_insert_first(data, size + sizeof(node));

    return rand_iterator<T>(
            data,
            static_cast<difference_type>(get_data_addr(nd) / sizeof(T)) // TODO!
            );
}

template <typename T>
inline void region::deallocate(rand_iterator<T> ptr)
{
    if (!free_list_initialized)
    {
        free_list_init(data, N);
        free_list_initialized = true;
    }

    volatile uint8_t* ptr8 = ptr.data() + ptr.pos() * sizeof(T);
    node n1 = get_node_from_ptr(ptr8 - sizeof(node));
    node n2 = next_node(data, n1);

    make_node(data, n1.pos, n1.size, n1.pred_pos, false);

    if (!n2.allocated)
        merge_nodes(data, n1, n2, false);

    n1 = get_node(data, n1.pos);
    node n3 = prev_node(data, n1);

    if (!n3.allocated && n1 != n3)
        merge_nodes(data, n3, n1, false);
}

} // namespace memory
} // namespace burst
