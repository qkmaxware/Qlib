/// </Summary>
/// This file contains shotcut names for fixed sized numeric types
/// Functionally it is similar to stdint.h but also contains fixed sized floating point number defintions
/// </Summary>

#ifndef _SIMPLETYPES_H
#define _SIMPLETYPES_H

#include <stdint.h>
#include <limits>

    //Compile time checking of an expression
    #define COMPILE_CHECK(expression) !(expression)
    #define COMPILE_ASSERT(expression, msg) typedef char msg[(expression) ? 1 : 0]

    //Default to nothing supported
    #define SUPPORTS_INT8 0
    #define SUPPORTS_INT16 0
    #define SUPPORTS_INT32 0
    #define SUPPORTS_INT64 0

    #define SUPPORTS_UNSIGNED8 0
    #define SUPPORTS_UNSIGNED16 0
    #define SUPPORTS_UNSIGNED32 0
    #define SUPPORTS_UNSIGNED64 0

    #define SUPPORTS_FLOAT128 0
    #define SUPPORTS_FLOAT64 0
    #define SUPPORTS_FLOAT32 0

    typedef int isize;  //System native sized integer
    typedef long int ilong;

    //Signed integers
    #ifdef INT8_MAX
        typedef int_least8_t i8; 
        #undef SUPPORTS_INT8
        #define SUPPORTS_INT8 1
    #endif
    #ifdef INT16_MAX
        typedef int_least16_t i16;
        #undef SUPPORTS_INT16
        #define SUPPORTS_INT16 1
    #endif
    #ifdef INT32_MAX
        typedef int_least32_t i32;
        #undef SUPPORTS_INT32
        #define SUPPORTS_INT32 1
    #endif
    #ifdef INT64_MAX
        typedef int_least64_t i64;
        #undef SUPPORTS_INT64
        #define SUPPORTS_INT64 1
    #endif

    typedef unsigned int usize; //System native sized unsigned integer
    typedef unsigned int uint;  //System native sized unsigned integer
    typedef long unsigned int ulong;

    //Unsigned integers
    #ifdef UINT8_MAX
        typedef uint_least8_t u8;
        #undef SUPPORTS_UNSIGNED8
        #define SUPPORTS_UNSIGNED8 1
    #endif
    #ifdef UINT16_MAX
        typedef uint_least16_t u16;
        #undef SUPPORTS_UNSIGNED16
        #define SUPPORTS_UNSIGNED16 1
    #endif
    #ifdef UINT32_MAX
        typedef uint_least32_t u32;
        #undef SUPPORTS_UNSIGNED32
        #define SUPPORTS_UNSIGNED32 
    #endif
    #ifdef UINT64_MAX
        typedef uint_least64_t u64;
        #undef SUPPORTS_UNSIGNED64
        #define SUPPORTS_UNSIGNED64 1
    #endif

    typedef float fsize;    //System native floating point value

    #define FLOAT_SIZE __SIZEOF_FLOAT__
    #define DOUBLE_SIZE __SIZEOF_DOUBLE__
    #define LONGDOUBLE_SIZE __SIZEOF_LONG_DOUBLE__
    #define BITS_PER_CHAR __CHAR_BIT__

    //Floating point values as determined here https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html (macros should not be used directly? so how should they be used?)
    //float, double, long double
    #if FLOAT_SIZE * BITS_PER_CHAR >= 128 && SUPPORTS_FLOAT128 == 0
        typedef float f128;
        #undef SUPPORTS_FLOAT128
        #define SUPPORTS_FLOAT128 1
    #endif
    #if DOUBLE_SIZE * BITS_PER_CHAR >= 128 && SUPPORTS_FLOAT128 == 0
        typedef double f128;
        #undef SUPPORTS_FLOAT128
        #define SUPPORTS_FLOAT128 1
    #endif
    #if LONGDOUBLE_SIZE * BITS_PER_CHAR >= 128 && SUPPORTS_FLOAT128 == 0
        typedef long double f128;
        #undef SUPPORTS_FLOAT128
        #define SUPPORTS_FLOAT128 1
    #endif

    #if FLOAT_SIZE * BITS_PER_CHAR >= 64 && SUPPORTS_FLOAT64 == 0
        typedef float f64;
        #undef SUPPORTS_FLOAT64
        #define SUPPORTS_FLOAT64 1
    #endif
    #if DOUBLE_SIZE * BITS_PER_CHAR >= 64 && SUPPORTS_FLOAT64 == 0
        typedef double f64;
        #undef SUPPORTS_FLOAT64
        #define SUPPORTS_FLOAT64 1
    #endif
    #if LONGDOUBLE_SIZE * BITS_PER_CHAR >= 64 && SUPPORTS_FLOAT64 == 0
        typedef long double f64;
        #undef SUPPORTS_FLOAT64
        #define SUPPORTS_FLOAT64 1
    #endif

    #if FLOAT_SIZE * __CHAR_BIT__ >= 32 && SUPPORTS_FLOAT32 == 0
        typedef float f32;
        #undef SUPPORTS_FLOAT32
        #define SUPPORTS_FLOAT32 1
    #endif
    #if DOUBLE_SIZE * __CHAR_BIT__ >= 32 && SUPPORTS_FLOAT32 == 0
        typedef double f32;
        #undef SUPPORTS_FLOAT32
        #define SUPPORTS_FLOAT32 1
    #endif
    #if LONGDOUBLE_SIZE * __CHAR_BIT__ >= 32 && SUPPORTS_FLOAT32 == 0
        typedef long double f32;
        #undef SUPPORTS_FLOAT32
        #define SUPPORTS_FLOAT32 1
    #endif

#endif
