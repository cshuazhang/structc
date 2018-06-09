﻿#ifndef _H_ATOM
#define _H_ATOM

#include "atomic.h"

/*
 * atom_t 自旋锁类型
 * atom_t o = 0 or static atom_t _m;
 * atom_lock(o); 
 * - One Man RPG
 * atom_unlock(o);
 */
typedef volatile long atom_t;

#ifdef __GNUC__

#define atom_trylock(o)     (!__sync_lock_test_and_set(&(o), 1))

#define atom_lock(o)        while(__sync_lock_test_and_set(&(o), 1))

#define atom_unlock(o)      __sync_lock_release(&(o))

// 保证代码不乱序
#define ATOM_SYNC()         __sync_synchronize()

// v += a ; return v;
#define ATOM_ADD(v, a)      __sync_add_and_fetch(&(v), (a))
// type tmp = v ; v = a; return tmp;
#define ATOM_SET(v, a)      __sync_lock_test_and_set(&(v), (a))
// v &= a; return v;
#define ATOM_AND(v, a)      __sync_and_and_fetch(&(v), (a))
// return ++v;
#define ATOM_INC(v)         __sync_add_and_fetch(&(v), 1)
// return --v;
#define ATOM_DEC(v)         __sync_sub_and_fetch(&(v), 1)
// bool b = v == c; b ? v=a : ; return b;
#define ATOM_CAS(v, c, a)   __sync_bool_compare_and_swap(&(v), (c), (a))

#endif

#ifdef _MSC_VER

#include <intrin.h>
#include <intrin0.h>

/* Interlocked intrinsic mapping for _nf/_acq/_rel */
#if defined(_M_ARM) || defined(_M_ARM64)
#define _ACQUIRE(x) ATOMIC_CONCAT(x, _acq)
#else /* defined(_M_ARM) || defined(_M_ARM64) */
#define _ACQUIRE(x) x
#endif /* defined(_M_ARM) || defined(_M_ARM64) */

#define atom_trylock(o)     (!_ACQUIRE(_interlockedbittestandset)(&(o), 0))

#define atom_lock(o)        while(_ACQUIRE(_interlockedbittestandset)(&(o), 0))

inline void store_release(atom_t * x) {
    /* store _Value atomically with release memory order */
#if defined(_M_ARM) || defined(_M_ARM64)
    __dmb(0xB /* _ARM_BARRIER_ISH or _ARM64_BARRIER_ISH*/);
    __iso_volatile_store32((volatile int *)x, 0);
#else
    _ReadWriteBarrier();
    *x = 0;
#endif
}

#define atom_unlock(o)      store_release(&(o))

// 保证代码不乱序优化后执行
#define ATOM_SYNC()         MemoryBarrier()

// v 和 a 都是 long 这样数据
#define ATOM_ADD(v, a)      InterlockedAdd((volatile LONG *)&(v), (LONG)(a))
#define ATOM_SET(v, a)      InterlockedExchange((volatile LONG *)&(v), (LONG)(a))
#define ATOM_AND(v, a)      InterlockedAnd((volatile LONG *)&(v), (LONG)(a))
#define ATOM_INC(v)         InterlockedIncrement((volatile LONG *)&(v))
#define ATOM_DEC(v)         InterlockedDecrement((volatile LONG *)&(v))
//
// 对于 InterlockedCompareExchange(v, c, a) 等价于下面
// long tmp = v ; v == a ? v = c : ; return tmp;
//
// 咱们的 ATOM_CAS(v, c, a) 等价于下面
// long tmp = v ; v == c ? v = a : ; return tmp;
//
#define ATOM_CAS(v, c, a)   ((LONG)(c) == InterlockedCompareExchange((volatile LONG *)&(v), (LONG)(a), (LONG)(c)))

#endif

#endif//_H_ATOM
