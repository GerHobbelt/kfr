/*
  Copyright (C) 2016 D Levin (https://www.kfrlib.com)
  This file is part of KFR

  KFR is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KFR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KFR.

  If GPL is not suitable for your project, you must purchase a commercial license to use KFR.
  Buying a commercial license is mandatory as soon as you develop commercial activities without
  disclosing the source code of your own applications.
  See https://www.kfrlib.com for details.
 */
#pragma once

#include "simd.hpp"

CMT_PRAGMA_GNU(GCC diagnostic push)
CMT_PRAGMA_GNU(GCC diagnostic ignored "-Wuninitialized")
CMT_PRAGMA_GNU(GCC diagnostic ignored "-Wpragmas")
CMT_PRAGMA_GNU(GCC diagnostic ignored "-Wunknown-warning-option")
CMT_PRAGMA_GNU(GCC diagnostic ignored "-Wmaybe-uninitialized")

namespace kfr
{

#if KFR_SHOW_NOT_OPTIMIZED
CMT_PUBLIC_C CMT_DLL_EXPORT void not_optimized(const char* fn) CMT_NOEXCEPT;
#else
#define not_optimized(...)                                                                                   \
    do                                                                                                       \
    {                                                                                                        \
    } while (0)
#endif

inline namespace CMT_ARCH_NAME
{

namespace intrinsics
{

template <typename T, size_t N>
using simd = typename simd_type<T, N>::type;

template <typename T, size_t N, typename U>
union simd_small_array {
    static_assert(sizeof(T) * N == sizeof(U), "");
    T arr[N];
    U whole;

    KFR_INTRINSIC static constexpr simd_small_array from(U whole)
    {
        union {
            const U w;
            simd_small_array r;
        } u{ whole };
        return u.r;
    }
};

#define KFR_SIMD_TYPE(T, N, ...)                                                                             \
    template <>                                                                                              \
    struct simd_type<T, N>                                                                                   \
    {                                                                                                        \
        using type = __VA_ARGS__;                                                                            \
    };

#define KFR_SIMD_SMALL_TYPE(T, N, U)                                                                         \
    template <>                                                                                              \
    struct simd_type<T, N>                                                                                   \
    {                                                                                                        \
        using type = simd_small_array<T, N, U>;                                                              \
    };

template <typename T>
struct simd_type<T, 1>
{
    using type = T;
};

template <typename T, size_t N>
struct simd_type
{
    using type = simd_halves<T, N>;
};

KFR_SIMD_SMALL_TYPE(u8, 2, u16)
KFR_SIMD_SMALL_TYPE(i8, 2, u16)

KFR_SIMD_SMALL_TYPE(u8, 4, u32)
KFR_SIMD_SMALL_TYPE(u16, 2, u32)
KFR_SIMD_SMALL_TYPE(i8, 4, u32)
KFR_SIMD_SMALL_TYPE(i16, 2, u32)

KFR_SIMD_SMALL_TYPE(u8, 8, u64)
KFR_SIMD_SMALL_TYPE(u16, 4, u64)
KFR_SIMD_SMALL_TYPE(u32, 2, u64)
KFR_SIMD_SMALL_TYPE(i8, 8, u64)
KFR_SIMD_SMALL_TYPE(i16, 4, u64)
KFR_SIMD_SMALL_TYPE(i32, 2, u64)

KFR_SIMD_SMALL_TYPE(f32, 2, f64)

#ifdef CMT_ARCH_SSE
KFR_SIMD_TYPE(f32, 4, __m128)
KFR_SIMD_TYPE(f64, 2, __m128d)
#endif // CMT_ARCH_SSE

#ifdef CMT_ARCH_SSE2
KFR_SIMD_TYPE(u8, 16, __m128i)
KFR_SIMD_TYPE(u16, 8, __m128i)
KFR_SIMD_TYPE(u32, 4, __m128i)
KFR_SIMD_TYPE(u64, 2, __m128i)
KFR_SIMD_TYPE(i8, 16, __m128i)
KFR_SIMD_TYPE(i16, 8, __m128i)
KFR_SIMD_TYPE(i32, 4, __m128i)
KFR_SIMD_TYPE(i64, 2, __m128i)
#endif // CMT_ARCH_SSE2

#ifdef CMT_ARCH_AVX
KFR_SIMD_TYPE(float, 8, __m256)
KFR_SIMD_TYPE(double, 4, __m256d)
KFR_SIMD_TYPE(u8, 32, __m256i)
KFR_SIMD_TYPE(u16, 16, __m256i)
KFR_SIMD_TYPE(u32, 8, __m256i)
KFR_SIMD_TYPE(u64, 4, __m256i)
KFR_SIMD_TYPE(i8, 32, __m256i)
KFR_SIMD_TYPE(i16, 16, __m256i)
KFR_SIMD_TYPE(i32, 8, __m256i)
KFR_SIMD_TYPE(i64, 4, __m256i)
#endif // CMT_ARCH_AVX

#ifdef CMT_ARCH_AVX512
KFR_SIMD_TYPE(float, 16, __m512)
KFR_SIMD_TYPE(double, 8, __m512d)
KFR_SIMD_TYPE(u8, 64, __m512i)
KFR_SIMD_TYPE(u16, 32, __m512i)
KFR_SIMD_TYPE(u32, 16, __m512i)
KFR_SIMD_TYPE(u64, 8, __m512i)
KFR_SIMD_TYPE(i8, 64, __m512i)
KFR_SIMD_TYPE(i16, 32, __m512i)
KFR_SIMD_TYPE(i32, 16, __m512i)
KFR_SIMD_TYPE(i64, 8, __m512i)
#endif // CMT_ARCH_AVX512

#ifdef CMT_ARCH_NEON
KFR_SIMD_TYPE(u8, 16, uint8x16_t);
KFR_SIMD_TYPE(u16, 8, uint16x8_t);
KFR_SIMD_TYPE(u32, 4, uint32x4_t);
KFR_SIMD_TYPE(u64, 2, uint64x2_t);
KFR_SIMD_TYPE(i8, 16, int8x16_t);
KFR_SIMD_TYPE(i16, 8, int16x8_t);
KFR_SIMD_TYPE(i32, 4, int32x4_t);
KFR_SIMD_TYPE(i64, 2, int64x2_t);
KFR_SIMD_TYPE(f32, 4, float32x4_t);
#ifdef CMT_ARCH_NEON64
KFR_SIMD_TYPE(f64, 2, float64x2_t);
#endif // CMT_ARCH_NEON64
#endif // CMT_ARCH_NEON

#if defined CMT_COMPILER_MSVC
#define KFR_i8sse_INDEX(x, i) x.m128i_i8[i]
#define KFR_i16sse_INDEX(x, i) x.m128i_i16[i]
#define KFR_i32sse_INDEX(x, i) x.m128i_i32[i]
#define KFR_i64sse_INDEX(x, i) x.m128i_i64[i]
#define KFR_u8sse_INDEX(x, i) x.m128i_u8[i]
#define KFR_u16sse_INDEX(x, i) x.m128i_u16[i]
#define KFR_u32sse_INDEX(x, i) x.m128i_u32[i]
#define KFR_u64sse_INDEX(x, i) x.m128i_u64[i]
#define KFR_f32sse_INDEX(x, i) x.m128_f32[i]
#define KFR_f64sse_INDEX(x, i) x.m128d_f64[i]
#else
#define KFR_i8sse_INDEX(x, i) bitcast_anything<simd_array<i8, 16>>(x).val[i]
#define KFR_i16sse_INDEX(x, i) bitcast_anything<simd_array<i16, 8>>(x).val[i]
#define KFR_i32sse_INDEX(x, i) _mm_cvtsi128_si32(_mm_shuffle_epi32(x, _MM_SHUFFLE(3, 2, 1, i)))
#define KFR_i64sse_INDEX(x, i) _mm_cvtsi128_si64(_mm_shuffle_epi32(x, _MM_SHUFFLE(3, 2, (i)*2 + 1, i * 2)))
#define KFR_u8sse_INDEX(x, i) bitcast_anything<simd_array<u8, 16>>(x).val[i]
#define KFR_u16sse_INDEX(x, i) bitcast_anything<simd_array<u16, 8>>(x).val[i]
#define KFR_u32sse_INDEX(x, i) _mm_cvtsi128_si32(_mm_shuffle_epi32(x, _MM_SHUFFLE(3, 2, 1, i)))
#define KFR_u64sse_INDEX(x, i) _mm_cvtsi128_si64(_mm_shuffle_epi32(x, _MM_SHUFFLE(3, 2, (i)*2 + 1, i * 2)))
#define KFR_f32sse_INDEX(x, i) _mm_cvtss_f32(_mm_shuffle_ps(x, x, _MM_SHUFFLE(3, 2, 1, i)))
#define KFR_f64sse_INDEX(x, i) _mm_cvtsd_f64(_mm_shuffle_pd(x, x, _MM_SHUFFLE2(1, i)))
#endif

// specializations

#ifdef KFR_NATIVE_INTRINSICS

#define KFR_GEN_ty(n, ty) ty(n)
#define KFR_GEN_arg_def(n, ty) ty arg##n
#define KFR_GEN_arg(n, ty) arg##n

#define KFR_INTRIN_MAKE(n, ty, intrin)                                                                       \
    KFR_INTRINSIC simd<ty, n> simd_make(ctype_t<ty>, CMT_GEN_LIST(n, KFR_GEN_arg_def, ty)) CMT_NOEXCEPT      \
    {                                                                                                        \
        return intrin(CMT_GEN_LIST(n, KFR_GEN_arg, ty));                                                     \
    }

#ifdef CMT_ARCH_SSE2
inline __m128i KFR_mm_setr_epi64x(int64_t q0, int64_t q1) CMT_NOEXCEPT { return _mm_set_epi64x(q1, q0); }
KFR_INTRIN_MAKE(2, i64, KFR_mm_setr_epi64x)
KFR_INTRIN_MAKE(2, u64, KFR_mm_setr_epi64x)
KFR_INTRIN_MAKE(2, f64, _mm_setr_pd)
KFR_INTRIN_MAKE(4, i32, _mm_setr_epi32)
KFR_INTRIN_MAKE(4, u32, _mm_setr_epi32)
KFR_INTRIN_MAKE(4, f32, _mm_setr_ps)
KFR_INTRIN_MAKE(8, i16, _mm_setr_epi16)
KFR_INTRIN_MAKE(8, u16, _mm_setr_epi16)
KFR_INTRIN_MAKE(16, i8, _mm_setr_epi8)
KFR_INTRIN_MAKE(16, u8, _mm_setr_epi8)

#define KFR_INTRIN_BITCAST(Tout, Tin, N, ...)                                                                \
    KFR_INTRINSIC simd<Tout, N> simd_bitcast(simd_cvt_t<Tout, Tin, N>, const simd<Tin, N>& x) CMT_NOEXCEPT   \
    {                                                                                                        \
        return __VA_ARGS__;                                                                                  \
    }
KFR_INTRIN_BITCAST(f32, i32, 4, _mm_castsi128_ps(x))
KFR_INTRIN_BITCAST(i32, f32, 4, _mm_castps_si128(x))
KFR_INTRIN_BITCAST(f64, i64, 2, _mm_castsi128_pd(x))
KFR_INTRIN_BITCAST(i64, f64, 2, _mm_castpd_si128(x))

#define KFR_INTRIN_BROADCAST(T, N, ...)                                                                      \
    KFR_INTRINSIC simd<T, N> simd_broadcast(simd_t<T, N>, T value) CMT_NOEXCEPT { return __VA_ARGS__; }

KFR_INTRIN_BROADCAST(i8, 16, _mm_set1_epi8(value))
KFR_INTRIN_BROADCAST(i16, 8, _mm_set1_epi16(value))
KFR_INTRIN_BROADCAST(i32, 4, _mm_set1_epi32(value))
KFR_INTRIN_BROADCAST(i64, 2, _mm_set1_epi64x(value))
KFR_INTRIN_BROADCAST(u8, 16, _mm_set1_epi8(value))
KFR_INTRIN_BROADCAST(u16, 8, _mm_set1_epi16(value))
KFR_INTRIN_BROADCAST(u32, 4, _mm_set1_epi32(value))
KFR_INTRIN_BROADCAST(u64, 2, _mm_set1_epi64x(value))
KFR_INTRIN_BROADCAST(f32, 4, _mm_set1_ps(value))
KFR_INTRIN_BROADCAST(f64, 2, _mm_set1_pd(value))

#define KFR_INTRIN_SHUFFLE_SWAP(T, N, ...)                                                                   \
    KFR_INTRINSIC simd<T, N> simd_shuffle(simd_t<T, N>, const simd<T, N>& x, csizeseq_t<N> ^ csize<1>,       \
                                          overload_priority<9>) CMT_NOEXCEPT                                 \
    {                                                                                                        \
        return __VA_ARGS__;                                                                                  \
    }

#define KFR_INTRIN_SHUFFLE_LINEAR(T, Nout, Nin, ...)                                                         \
    KFR_INTRINSIC simd<T, Nout> simd_shuffle(simd_t<T, Nin>, const simd<T, Nin>& x, csizeseq_t<Nout>,        \
                                             overload_priority<9>) CMT_NOEXCEPT                              \
    {                                                                                                        \
        return __VA_ARGS__;                                                                                  \
    }
#define KFR_INTRIN_SHUFFLE_LINEAR_START(T, Nout, Nin, Nstart, ...)                                           \
    KFR_INTRINSIC simd<T, Nout> simd_shuffle(simd_t<T, Nin>, const simd<T, Nin>& x,                          \
                                             csizeseq_t<Nout, Nstart>, overload_priority<9>) CMT_NOEXCEPT    \
    {                                                                                                        \
        return __VA_ARGS__;                                                                                  \
    }

#define KFR_INTRIN_SHUFFLE_CONCAT(T, Nin, ...)                                                               \
    KFR_INTRINSIC simd<T, Nin + Nin> simd_shuffle(simd2_t<T, Nin, Nin>, const simd<T, Nin>& x,               \
                                                  const simd<T, Nin>& y, csizeseq_t<Nin + Nin>,              \
                                                  overload_priority<9>) CMT_NOEXCEPT                         \
    {                                                                                                        \
        return __VA_ARGS__;                                                                                  \
    }

// extend
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16, 1, _mm_cvtsi32_si128(u8(x)))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8, 1, _mm_cvtsi32_si128(u16(x)))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4, 1, _mm_cvtsi32_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2, 1, _mm_cvtsi64_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16, 1, _mm_cvtsi32_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8, 1, _mm_cvtsi32_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4, 1, _mm_cvtsi32_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u64, 2, 1, _mm_cvtsi64_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(f32, 4, 1, _mm_set_ss(x))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 2, 1, _mm_set_sd(x))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16, 2, _mm_cvtsi32_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16, 2, _mm_cvtsi32_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16, 4, _mm_cvtsi32_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16, 4, _mm_cvtsi32_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16, 8, _mm_cvtsi64_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16, 8, _mm_cvtsi64_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8, 2, _mm_cvtsi32_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8, 2, _mm_cvtsi32_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8, 4, _mm_cvtsi64_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8, 4, _mm_cvtsi64_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4, 2, _mm_cvtsi64_si128(x.whole))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4, 2, _mm_cvtsi64_si128(x.whole))

// slice
KFR_INTRIN_SHUFFLE_LINEAR(i32, 1, 4, _mm_cvtsi128_si32(x))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 1, 4, _mm_cvtsi128_si32(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 1, 2, _mm_cvtsi128_si64(x))
KFR_INTRIN_SHUFFLE_LINEAR(u64, 1, 2, _mm_cvtsi128_si64(x))
KFR_INTRIN_SHUFFLE_LINEAR(f32, 1, 4, _mm_cvtss_f32(x))
KFR_INTRIN_SHUFFLE_LINEAR(f32, 2, 4, bitcast_anything<simd<float, 2>>(_mm_cvtsd_f64(_mm_castps_pd(x))))
KFR_INTRIN_SHUFFLE_LINEAR(f32, 4, 2, _mm_castpd_ps(_mm_set_sd(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 1, 2, _mm_cvtsd_f64(x))

KFR_INTRIN_SHUFFLE_LINEAR(i8, 2, 16, simd<i8, 2>::from(u16(_mm_cvtsi128_si32(x))))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 4, 16, simd<i8, 4>::from(_mm_cvtsi128_si32(x)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 8, 16, simd<i8, 8>::from(_mm_cvtsi128_si64(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 2, 16, simd<u8, 2>::from(u16(_mm_cvtsi128_si32(x))))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 4, 16, simd<u8, 4>::from(_mm_cvtsi128_si32(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 8, 16, simd<u8, 8>::from(_mm_cvtsi128_si64(x)))

KFR_INTRIN_SHUFFLE_LINEAR(i16, 2, 8, simd<i16, 2>::from(_mm_cvtsi128_si32(x)))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 4, 8, simd<i16, 4>::from(_mm_cvtsi128_si64(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 2, 8, simd<u16, 2>::from(_mm_cvtsi128_si32(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 4, 8, simd<u16, 4>::from(_mm_cvtsi128_si64(x)))

KFR_INTRIN_SHUFFLE_LINEAR(i32, 2, 4, simd<i32, 2>::from(_mm_cvtsi128_si64(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 2, 4, simd<u32, 2>::from(_mm_cvtsi128_si64(x)))

// high
KFR_INTRIN_SHUFFLE_LINEAR_START(u8, 8, 16, 8, simd<u8, 8>::from(KFR_u64sse_INDEX(x, 1)))
KFR_INTRIN_SHUFFLE_LINEAR_START(i8, 8, 16, 8, simd<i8, 8>::from(KFR_u64sse_INDEX(x, 1)))
KFR_INTRIN_SHUFFLE_LINEAR_START(u16, 4, 8, 4, simd<u16, 4>::from(KFR_u64sse_INDEX(x, 1)))
KFR_INTRIN_SHUFFLE_LINEAR_START(i16, 4, 8, 4, simd<i16, 4>::from(KFR_u64sse_INDEX(x, 1)))
KFR_INTRIN_SHUFFLE_LINEAR_START(u32, 2, 4, 2, simd<u32, 2>::from(KFR_u64sse_INDEX(x, 1)))
KFR_INTRIN_SHUFFLE_LINEAR_START(i32, 2, 4, 2, simd<i32, 2>::from(KFR_u64sse_INDEX(x, 1)))

#define KFR_INTRIN_CONVERT(Tout, Tin, N, ...)                                                                \
    KFR_INTRINSIC simd<Tout, N> simd_convert(simd_cvt_t<Tout, Tin, N>, const simd<Tin, N>& x) CMT_NOEXCEPT   \
    {                                                                                                        \
        return __VA_ARGS__;                                                                                  \
    }

KFR_INTRIN_CONVERT(f32, i32, 4, _mm_cvtepi32_ps(x))
KFR_INTRIN_CONVERT(i32, f32, 4, _mm_cvttps_epi32(x))
KFR_INTRIN_CONVERT(i32, f64, 2, simd<i32, 2>::from(_mm_cvtsi128_si64(_mm_cvttpd_epi32(x))))
KFR_INTRIN_CONVERT(f64, i32, 2, _mm_cvtepi32_pd(KFR_mm_setr_epi64x(x.whole, 0)))
KFR_INTRIN_CONVERT(i64, f64, 2, _mm_set_epi64x(_mm_cvttsd_si64(_mm_unpackhi_pd(x, x)), _mm_cvttsd_si64(x)))
KFR_INTRIN_CONVERT(f64, i64, 2,
                   _mm_unpacklo_pd(_mm_cvtsi64_sd(_mm_setzero_pd(), _mm_cvtsi128_si64(x)),
                                   _mm_cvtsi64_sd(_mm_setzero_pd(), KFR_i64sse_INDEX(x, 1))))
#ifdef CMT_ARCH_AVX
KFR_INTRIN_CONVERT(f64, f32, 4, _mm256_cvtps_pd(x))
#else
KFR_INTRIN_CONVERT(f64, f32, 4,
                   simd<f64, 4>{ _mm_cvtps_pd(x),
                                 _mm_cvtps_pd(_mm_shuffle_ps(x, x, _MM_SHUFFLE(1, 0, 3, 2))) })
#endif
#ifdef CMT_ARCH_AVX
KFR_INTRIN_CONVERT(f32, f64, 4, _mm256_cvtpd_ps(x))
#else
KFR_INTRIN_CONVERT(f32, f64, 4,
                   simd<f32, 4>{ _mm_castpd_ps(_mm_unpacklo_pd(_mm_castps_pd(_mm_cvtpd_ps(x.low)),
                                                               _mm_castps_pd(_mm_cvtpd_ps(x.high)))) })
#endif
#endif // CMT_ARCH_SSE2

#ifdef CMT_ARCH_SSE41

KFR_INTRIN_CONVERT(i16, i8, 8, _mm_cvtepi8_epi16(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(u16, u8, 8, _mm_cvtepu8_epi16(_mm_cvtsi64_si128(x.whole)))

KFR_INTRIN_CONVERT(i32, i16, 4, _mm_cvtepi16_epi32(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(u32, u16, 4, _mm_cvtepu16_epi32(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(i32, i8, 4, _mm_cvtepi8_epi32(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_CONVERT(u32, u8, 4, _mm_cvtepu8_epi32(_mm_cvtsi32_si128(x.whole)))

KFR_INTRIN_CONVERT(i64, i32, 2, _mm_cvtepi32_epi64(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(u64, u32, 2, _mm_cvtepu32_epi64(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(i64, i16, 2, _mm_cvtepi16_epi64(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_CONVERT(u64, u16, 2, _mm_cvtepu16_epi64(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_CONVERT(i64, i8, 2, _mm_cvtepi8_epi64(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_CONVERT(u64, u8, 2, _mm_cvtepu8_epi64(_mm_cvtsi32_si128(x.whole)))

KFR_INTRIN_CONVERT(f32, i8, 4, _mm_cvtepi32_ps(_mm_cvtepi8_epi32(_mm_cvtsi32_si128(x.whole))))
KFR_INTRIN_CONVERT(f32, i16, 4, _mm_cvtepi32_ps(_mm_cvtepi16_epi32(_mm_cvtsi64_si128(x.whole))))
KFR_INTRIN_CONVERT(f32, u8, 4, _mm_cvtepi32_ps(_mm_cvtepu8_epi32(_mm_cvtsi32_si128(x.whole))))
KFR_INTRIN_CONVERT(f32, u16, 4, _mm_cvtepi32_ps(_mm_cvtepu16_epi32(_mm_cvtsi64_si128(x.whole))))

#ifndef CMT_ARCH_AVX
KFR_INTRIN_CONVERT(i64, i32, 4,
                   simd<i64, 4>{ _mm_cvtepi32_epi64(x),
                                 _mm_cvtepi32_epi64(_mm_shuffle_epi32(x, _MM_SHUFFLE(1, 0, 3, 2))) })
#endif
#endif

#ifdef CMT_ARCH_AVX
KFR_INTRIN_MAKE(4, f64, _mm256_setr_pd)
KFR_INTRIN_MAKE(8, f32, _mm256_setr_ps)

KFR_INTRIN_BITCAST(f32, i32, 8, _mm256_castsi256_ps(x))

KFR_INTRIN_BITCAST(i32, f32, 8, _mm256_castps_si256(x))
KFR_INTRIN_BITCAST(f64, i64, 4, _mm256_castsi256_pd(x))
KFR_INTRIN_BITCAST(i64, f64, 4, _mm256_castpd_si256(x))

KFR_INTRINSIC __m256 KFR_mm256_setr_m128(__m128 x, __m128 y)
{
    return _mm256_insertf128_ps(_mm256_castps128_ps256(x), y, 1);
}

KFR_INTRINSIC __m256d KFR_mm256_setr_m128d(__m128d x, __m128d y)
{
    return _mm256_insertf128_pd(_mm256_castpd128_pd256(x), y, 1);
}
KFR_INTRINSIC __m256i KFR_mm256_setr_m128i(__m128i x, __m128i y)
{
#ifdef CMT_ARCH_AVX2
    return _mm256_inserti128_si256(_mm256_castsi128_si256(x), y, 1);
#else
    return _mm256_castps_si256(
        _mm256_insertf128_ps(_mm256_castps128_ps256(_mm_castsi128_ps(x)), _mm_castsi128_ps(y), 1));
#endif
}

KFR_INTRIN_SHUFFLE_CONCAT(f32, 4, KFR_mm256_setr_m128(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(f64, 2, KFR_mm256_setr_m128d(x, y))

// concat
KFR_INTRIN_SHUFFLE_CONCAT(i8, 16, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(i16, 8, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(i32, 4, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(i64, 2, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u8, 16, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u16, 8, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u32, 4, KFR_mm256_setr_m128i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u64, 2, KFR_mm256_setr_m128i(x, y))
// low
KFR_INTRIN_SHUFFLE_LINEAR(f32, 4, 8, _mm256_castps256_ps128(x))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 2, 4, _mm256_castpd256_pd128(x))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16, 32, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8, 16, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4, 8, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2, 4, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16, 32, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8, 16, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4, 8, _mm256_castsi256_si128(x))
KFR_INTRIN_SHUFFLE_LINEAR(u64, 2, 4, _mm256_castsi256_si128(x))

// extend
KFR_INTRIN_SHUFFLE_LINEAR(f32, 4 * 2, 4, _mm256_castps128_ps256(x))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 2 * 2, 2, _mm256_castpd128_pd256(x))

// high
KFR_INTRIN_SHUFFLE_LINEAR_START(f32, 4, 8, 4, _mm256_extractf128_ps(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(f64, 2, 4, 2, _mm256_extractf128_pd(x, 1))

KFR_INTRIN_BROADCAST(f32, 8, _mm256_set1_ps(value))
KFR_INTRIN_BROADCAST(f64, 4, _mm256_set1_pd(value))

KFR_INTRIN_SHUFFLE_LINEAR(f32, 8, 1, _mm256_castps128_ps256(_mm_set_ss(x)))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 4, 1, _mm256_castpd128_pd256(_mm_set_sd(x)))
#endif // CMT_ARCH_AVX

#ifdef CMT_ARCH_AVX2
KFR_INTRIN_MAKE(4, i64, _mm256_setr_epi64x)
KFR_INTRIN_MAKE(4, u64, _mm256_setr_epi64x)
KFR_INTRIN_MAKE(8, i32, _mm256_setr_epi32)
KFR_INTRIN_MAKE(8, u32, _mm256_setr_epi32)
KFR_INTRIN_MAKE(16, i16, _mm256_setr_epi16)
KFR_INTRIN_MAKE(16, u16, _mm256_setr_epi16)
KFR_INTRIN_MAKE(32, i8, _mm256_setr_epi8)
KFR_INTRIN_MAKE(32, u8, _mm256_setr_epi8)

KFR_INTRIN_CONVERT(i16, i8, 16, _mm256_cvtepi8_epi16(x))
KFR_INTRIN_CONVERT(u16, u8, 16, _mm256_cvtepu8_epi16(x))

KFR_INTRIN_CONVERT(i32, i16, 8, _mm256_cvtepi16_epi32(x))
KFR_INTRIN_CONVERT(u32, u16, 8, _mm256_cvtepu16_epi32(x))
KFR_INTRIN_CONVERT(i32, i8, 8, _mm256_cvtepi8_epi32(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(u32, u8, 8, _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(x.whole)))

KFR_INTRIN_CONVERT(i64, i32, 4, _mm256_cvtepi32_epi64(x))
KFR_INTRIN_CONVERT(u64, u32, 4, _mm256_cvtepu32_epi64(x))
KFR_INTRIN_CONVERT(i64, i16, 4, _mm256_cvtepi16_epi64(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(u64, u16, 4, _mm256_cvtepu16_epi64(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_CONVERT(i64, i8, 4, _mm256_cvtepi8_epi64(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_CONVERT(u64, u8, 4, _mm256_cvtepu8_epi64(_mm_cvtsi32_si128(x.whole)))

KFR_INTRIN_CONVERT(f32, i8, 8, _mm256_cvtepi32_ps(_mm256_cvtepi8_epi32(_mm_cvtsi64_si128(x.whole))))
KFR_INTRIN_CONVERT(f32, i16, 8, _mm256_cvtepi32_ps(_mm256_cvtepi16_epi32(x)))
KFR_INTRIN_CONVERT(f32, u8, 8, _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_cvtsi64_si128(x.whole))))
KFR_INTRIN_CONVERT(f32, u16, 8, _mm256_cvtepi32_ps(_mm256_cvtepu16_epi32(x)))

KFR_INTRIN_SHUFFLE_LINEAR_START(i8, 16, 32, 16, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(i16, 8, 16, 8, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(i32, 4, 8, 4, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(i64, 2, 4, 2, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(u8, 16, 32, 16, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(u16, 8, 16, 8, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(u32, 4, 8, 4, _mm256_extracti128_si256(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(u64, 2, 4, 2, _mm256_extracti128_si256(x, 1))

KFR_INTRIN_BROADCAST(i8, 32, _mm256_set1_epi8(value))
KFR_INTRIN_BROADCAST(i16, 16, _mm256_set1_epi16(value))
KFR_INTRIN_BROADCAST(i32, 8, _mm256_set1_epi32(value))
KFR_INTRIN_BROADCAST(i64, 4, _mm256_set1_epi64x(value))
KFR_INTRIN_BROADCAST(u8, 32, _mm256_set1_epi8(value))
KFR_INTRIN_BROADCAST(u16, 16, _mm256_set1_epi16(value))
KFR_INTRIN_BROADCAST(u32, 8, _mm256_set1_epi32(value))
KFR_INTRIN_BROADCAST(u64, 4, _mm256_set1_epi64x(value))

KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 2, 16, _mm256_castsi128_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 2, 8, _mm256_castsi128_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 2, 4, _mm256_castsi128_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2 * 2, 2, _mm256_castsi128_si256(x))

KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi32_si128(u8(x))))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi32_si128(u16(x))))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi64_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u64, 2 * 2, 1, _mm256_castsi128_si256(_mm_cvtsi64_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 2, 2, _mm256_castsi128_si256(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 2, 2, _mm256_castsi128_si256(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 2, 4, _mm256_castsi128_si256(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 2, 4, _mm256_castsi128_si256(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 2, 8, _mm256_castsi128_si256(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 2, 8, _mm256_castsi128_si256(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 2, 2, _mm256_castsi128_si256(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 2, 2, _mm256_castsi128_si256(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 2, 4, _mm256_castsi128_si256(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 2, 4, _mm256_castsi128_si256(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4 * 2, 2, _mm256_castsi128_si256(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 2, 2, _mm256_castsi128_si256(_mm_cvtsi64_si128(x.whole)))

KFR_INTRIN_CONVERT(i32, f32, 8, _mm256_cvttps_epi32(x))
KFR_INTRIN_CONVERT(f32, i32, 8, _mm256_cvtepi32_ps(x))
KFR_INTRIN_CONVERT(f64, i32, 4, _mm256_cvtepi32_pd(x))
KFR_INTRIN_CONVERT(i32, f64, 4, _mm256_cvttpd_epi32(x))
#endif // CMT_ARCH_AVX2

#ifdef CMT_ARCH_AVX512

static inline __m512d KFR_mm512_setr_pd(f64 x0, f64 x1, f64 x2, f64 x3, f64 x4, f64 x5, f64 x6, f64 x7)
{
    return _mm512_set_pd(x7, x6, x5, x4, x3, x2, x1, x0);
}
static inline __m512 KFR_mm512_setr_ps(f32 x0, f32 x1, f32 x2, f32 x3, f32 x4, f32 x5, f32 x6, f32 x7, f32 x8,
                                       f32 x9, f32 x10, f32 x11, f32 x12, f32 x13, f32 x14, f32 x15)
{
    return _mm512_set_ps(x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1, x0);
}
static inline __m512i KFR_mm512_setr_epi64(i64 x0, i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7)
{
    return _mm512_set_epi64(x7, x6, x5, x4, x3, x2, x1, x0);
}
static inline __m512i KFR_mm512_setr_epi32(i32 x0, i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7,
                                           i32 x8, i32 x9, i32 x10, i32 x11, i32 x12, i32 x13, i32 x14,
                                           i32 x15)
{
    return _mm512_set_epi32(x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1, x0);
}
static inline __m512i KFR_mm512_setr_epi16(i16 x0, i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7,
                                           i16 x8, i16 x9, i16 x10, i16 x11, i16 x12, i16 x13, i16 x14,
                                           i16 x15, i16 x16, i16 x17, i16 x18, i16 x19, i16 x20, i16 x21,
                                           i16 x22, i16 x23, i16 x24, i16 x25, i16 x26, i16 x27, i16 x28,
                                           i16 x29, i16 x30, i16 x31)
{
    return _mm512_set_epi16(x31, x30, x29, x28, x27, x26, x25, x24, x23, x22, x21, x20, x19, x18, x17, x16,
                            x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1, x0);
}
static inline __m512i KFR_mm512_setr_epi8(i8 x0, i8 x1, i8 x2, i8 x3, i8 x4, i8 x5, i8 x6, i8 x7, i8 x8,
                                          i8 x9, i8 x10, i8 x11, i8 x12, i8 x13, i8 x14, i8 x15, i8 x16,
                                          i8 x17, i8 x18, i8 x19, i8 x20, i8 x21, i8 x22, i8 x23, i8 x24,
                                          i8 x25, i8 x26, i8 x27, i8 x28, i8 x29, i8 x30, i8 x31, i8 x32,
                                          i8 x33, i8 x34, i8 x35, i8 x36, i8 x37, i8 x38, i8 x39, i8 x40,
                                          i8 x41, i8 x42, i8 x43, i8 x44, i8 x45, i8 x46, i8 x47, i8 x48,
                                          i8 x49, i8 x50, i8 x51, i8 x52, i8 x53, i8 x54, i8 x55, i8 x56,
                                          i8 x57, i8 x58, i8 x59, i8 x60, i8 x61, i8 x62, i8 x63)
{
    return _mm512_set_epi8(x63, x62, x61, x60, x59, x58, x57, x56, x55, x54, x53, x52, x51, x50, x49, x48,
                           x47, x46, x45, x44, x43, x42, x41, x40, x39, x38, x37, x36, x35, x34, x33, x32,
                           x31, x30, x29, x28, x27, x26, x25, x24, x23, x22, x21, x20, x19, x18, x17, x16,
                           x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1, x0);
}

KFR_INTRINSIC __m512 KFR_mm512_setr_m256(__m256 x, __m256 y)
{
    return _mm512_insertf32x8(_mm512_castps256_ps512(x), y, 1);
}

KFR_INTRINSIC __m512d KFR_mm512_setr_m256d(__m256d x, __m256d y)
{
    return _mm512_insertf64x4(_mm512_castpd256_pd512(x), y, 1);
}
KFR_INTRINSIC __m512i KFR_mm512_setr_m256i(__m256i x, __m256i y)
{
    return _mm512_inserti32x8(_mm512_castsi256_si512(x), y, 1);
}

KFR_INTRIN_MAKE(8, f64, KFR_mm512_setr_pd)
KFR_INTRIN_MAKE(16, f32, KFR_mm512_setr_ps)

KFR_INTRIN_MAKE(8, i64, KFR_mm512_setr_epi64)
KFR_INTRIN_MAKE(8, u64, KFR_mm512_setr_epi64)
KFR_INTRIN_MAKE(16, i32, KFR_mm512_setr_epi32)
KFR_INTRIN_MAKE(16, u32, KFR_mm512_setr_epi32)
KFR_INTRIN_MAKE(32, i16, KFR_mm512_setr_epi16)
KFR_INTRIN_MAKE(32, u16, KFR_mm512_setr_epi16)
KFR_INTRIN_MAKE(64, i8, KFR_mm512_setr_epi8)
KFR_INTRIN_MAKE(64, u8, KFR_mm512_setr_epi8)

KFR_INTRIN_BROADCAST(f32, 16, _mm512_set1_ps(value))
KFR_INTRIN_BROADCAST(f64, 8, _mm512_set1_pd(value))

KFR_INTRIN_BROADCAST(i8, 64, _mm512_set1_epi8(value))
KFR_INTRIN_BROADCAST(i16, 32, _mm512_set1_epi16(value))
KFR_INTRIN_BROADCAST(i32, 16, _mm512_set1_epi32(value))
KFR_INTRIN_BROADCAST(i64, 8, _mm512_set1_epi64(value))
KFR_INTRIN_BROADCAST(u8, 64, _mm512_set1_epi8(value))
KFR_INTRIN_BROADCAST(u16, 32, _mm512_set1_epi16(value))
KFR_INTRIN_BROADCAST(u32, 16, _mm512_set1_epi32(value))
KFR_INTRIN_BROADCAST(u64, 8, _mm512_set1_epi64(value))

KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi32_si128(u8(x))))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi32_si128(u16(x))))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi64_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi32_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u64, 2 * 4, 1, _mm512_castsi128_si512(_mm_cvtsi64_si128(x)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 4, 2, _mm512_castsi128_si512(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 4, 2, _mm512_castsi128_si512(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 4, 4, _mm512_castsi128_si512(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 4, 4, _mm512_castsi128_si512(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 4, 8, _mm512_castsi128_si512(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 4, 8, _mm512_castsi128_si512(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 4, 2, _mm512_castsi128_si512(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 4, 2, _mm512_castsi128_si512(_mm_cvtsi32_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 4, 4, _mm512_castsi128_si512(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 4, 4, _mm512_castsi128_si512(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4 * 4, 2, _mm512_castsi128_si512(_mm_cvtsi64_si128(x.whole)))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 4, 2, _mm512_castsi128_si512(_mm_cvtsi64_si128(x.whole)))

KFR_INTRIN_CONVERT(i32, f32, 16, _mm512_cvttps_epi32(x))
KFR_INTRIN_CONVERT(f32, i32, 16, _mm512_cvtepi32_ps(x))
KFR_INTRIN_CONVERT(f64, i32, 8, _mm512_cvtepi32_pd(x))
KFR_INTRIN_CONVERT(i32, f64, 8, _mm512_cvttpd_epi32(x))

KFR_INTRIN_SHUFFLE_LINEAR(f32, 4 * 4, 4, _mm512_castps128_ps512(x))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 2 * 4, 2, _mm512_castpd128_pd512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 4, 16, _mm512_castsi128_si512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 4, 8, _mm512_castsi128_si512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 4, 4, _mm512_castsi128_si512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2 * 4, 2, _mm512_castsi128_si512(x))

KFR_INTRIN_SHUFFLE_LINEAR(f32, 4 * 4, 2 * 4, _mm512_castps256_ps512(x))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 2 * 4, 2 * 2, _mm512_castpd256_pd512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 4, 2 * 16, _mm512_castsi256_si512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 4, 2 * 8, _mm512_castsi256_si512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 4, 2 * 4, _mm512_castsi256_si512(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2 * 4, 2 * 2, _mm512_castsi256_si512(x))

// low
KFR_INTRIN_SHUFFLE_LINEAR(f32, 4 * 2, 8 * 2, _mm512_castps512_ps256(x))
KFR_INTRIN_SHUFFLE_LINEAR(f64, 2 * 2, 4 * 2, _mm512_castpd512_pd256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i8, 16 * 2, 32 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i16, 8 * 2, 16 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i32, 4 * 2, 8 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(i64, 2 * 2, 4 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(u8, 16 * 2, 32 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(u16, 8 * 2, 16 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(u32, 4 * 2, 8 * 2, _mm512_castsi512_si256(x))
KFR_INTRIN_SHUFFLE_LINEAR(u64, 2 * 2, 4 * 2, _mm512_castsi512_si256(x))

// high
KFR_INTRIN_SHUFFLE_LINEAR_START(f32, 4 * 2, 8 * 2, 4 * 2, _mm512_extractf32x8_ps(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(f64, 2 * 2, 4 * 2, 2 * 2, _mm512_extractf64x4_pd(x, 1))

KFR_INTRIN_SHUFFLE_LINEAR_START(i32, 4 * 2, 8 * 2, 4 * 2, _mm512_extracti32x8_epi32(x, 1))
KFR_INTRIN_SHUFFLE_LINEAR_START(i64, 2 * 2, 4 * 2, 2 * 2, _mm512_extracti64x4_epi64(x, 1))

// concat
KFR_INTRIN_SHUFFLE_CONCAT(f32, 4 * 2, KFR_mm512_setr_m256(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(f64, 2 * 2, KFR_mm512_setr_m256d(x, y))

KFR_INTRIN_SHUFFLE_CONCAT(i8, 16 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(i16, 8 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(i32, 4 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(i64, 2 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u8, 16 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u16, 8 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u32, 4 * 2, KFR_mm512_setr_m256i(x, y))
KFR_INTRIN_SHUFFLE_CONCAT(u64, 2 * 2, KFR_mm512_setr_m256i(x, y))
#endif

#endif

// generic functions

template <typename T, size_t N1>
KFR_INTRINSIC const simd<T, N1>& simd_concat(const simd<T, N1>& x) CMT_NOEXCEPT;

template <typename T, size_t N1, size_t N2, size_t... Ns, size_t Nscount = csum(csizes<Ns...>)>
KFR_INTRINSIC simd<T, N1 + N2 + Nscount> simd_concat(const simd<T, N1>& x, const simd<T, N2>& y,
                                                     const simd<T, Ns>&... z) CMT_NOEXCEPT;

template <typename T, size_t N>
KFR_INTRINSIC simd_array<T, N> to_simd_array(const simd<T, N>& x) CMT_NOEXCEPT
{
    return bitcast_anything<simd_array<T, N>>(x);
}

template <typename T, size_t N>
KFR_INTRINSIC simd<T, N> from_simd_array(const simd_array<T, N>& x) CMT_NOEXCEPT
{
    return bitcast_anything<simd<T, N>>(x);
}

#define KFR_COMPONENTWISE_RET(code)                                                                          \
    vec<T, N> result;                                                                                        \
    for (size_t i = 0; i < N; i++)                                                                           \
        code;                                                                                                \
    return result;

#define KFR_COMPONENTWISE_RET_I(Tvec, code)                                                                  \
    Tvec result;                                                                                             \
    for (size_t i = 0; i < result.size(); i++)                                                               \
        code;                                                                                                \
    return result;

#define KFR_COMPONENTWISE(code)                                                                              \
    for (size_t i = 0; i < N; i++)                                                                           \
        code;

template <typename Tout>
KFR_INTRINSIC void simd_make(ctype_t<Tout>) CMT_NOEXCEPT = delete;

template <typename Tout, typename Arg>
KFR_INTRINSIC simd<Tout, 1> simd_make(ctype_t<Tout>, const Arg& arg) CMT_NOEXCEPT
{
    return simd<Tout, 1>{ static_cast<Tout>(arg) };
}

template <typename T, size_t... indices, typename... Args, size_t N = sizeof...(indices)>
KFR_INTRINSIC simd<T, N> simd_make_helper(csizes_t<indices...>, const Args&... args) CMT_NOEXCEPT;

template <typename Tout, typename... Args, size_t N = sizeof...(Args), KFR_ENABLE_IF(N > 1)>
KFR_INTRINSIC simd<Tout, N> simd_make(ctype_t<Tout>, const Args&... args) CMT_NOEXCEPT
{
    constexpr size_t Nlow = prev_poweroftwo(N - 1);
    return simd_concat<Tout, Nlow, N - Nlow>(simd_make_helper<Tout>(csizeseq<Nlow>, args...),
                                             simd_make_helper<Tout>(csizeseq<N - Nlow, Nlow>, args...));
}

template <typename T, size_t... indices, typename... Args, size_t N>
KFR_INTRINSIC simd<T, N> simd_make_helper(csizes_t<indices...>, const Args&... args) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    const T temp[] = { static_cast<T>(args)... };
    return simd_make(ctype<T>, temp[indices]...);
}

/// @brief Returns vector with undefined value
template <typename Tout, size_t N>
KFR_INTRINSIC simd<Tout, N> simd_undefined() CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    simd<Tout, N> x;
    return x;
}

/// @brief Returns vector with all zeros
template <typename Tout, size_t N>
KFR_INTRINSIC simd<Tout, N> simd_zeros() CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    return from_simd_array<Tout, N>({ Tout() });
}

/// @brief Returns vector with all ones
template <typename Tout, size_t N>
KFR_INTRINSIC simd<Tout, N> simd_allones() CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    simd_array<Tout, N> x{};
    KFR_COMPONENTWISE(x.val[i] = special_constants<Tout>::allones());
    return from_simd_array(x);
}

/// @brief Converts input vector to vector with subtype Tout
template <typename Tout, typename Tin, size_t N, size_t Nout = (sizeof(Tin) * N / sizeof(Tout)),
          KFR_ENABLE_IF(Nout == 1 || N == 1)>
KFR_INTRINSIC simd<Tout, Nout> simd_bitcast(simd_cvt_t<Tout, Tin, N>, const simd<Tin, N>& x) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    return bitcast_anything<simd<Tout, Nout>>(x);
}

/// @brief Converts input vector to vector with subtype Tout
template <typename Tout, typename Tin, size_t N, size_t Nout = (sizeof(Tin) * N / sizeof(Tout)),
          KFR_ENABLE_IF(Nout > 1 && N > 1)>
KFR_INTRINSIC simd<Tout, Nout> simd_bitcast(simd_cvt_t<Tout, Tin, N>, const simd<Tin, N>& x) CMT_NOEXCEPT
{
    constexpr size_t Nlow = prev_poweroftwo(N - 1);
    return simd_concat<Tout, Nlow * Nout / N, (N - Nlow) * Nout / N>(
        simd_bitcast(simd_cvt_t<Tout, Tin, Nlow>{},
                     simd_shuffle(simd_t<Tin, N>{}, x, csizeseq<Nlow>, overload_auto)),
        simd_bitcast(simd_cvt_t<Tout, Tin, N - Nlow>{},
                     simd_shuffle(simd_t<Tin, N>{}, x, csizeseq<N - Nlow, Nlow>, overload_auto)));
}

template <typename T, size_t N>
KFR_INTRINSIC const simd<T, N>& simd_bitcast(simd_cvt_t<T, T, N>, const simd<T, N>& x) CMT_NOEXCEPT
{
    return x;
}

template <typename T, size_t N, size_t index>
KFR_INTRINSIC T simd_get_element(const simd<T, N>& value, csize_t<index>) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    return to_simd_array<T, N>(value).val[index];
}

template <typename T, size_t N, size_t index>
KFR_INTRINSIC simd<T, N> simd_set_element(simd<T, N> value, csize_t<index>, T x) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    simd_array<T, N> arr = to_simd_array<T, N>(value);
    arr.val[index]       = x;
    return from_simd_array(arr);
}

template <typename T, size_t N>
KFR_INTRINSIC const simd<T, N>& simd_shuffle(simd_t<T, N>, const simd<T, N>& x, csizeseq_t<N>,
                                             overload_priority<10>) CMT_NOEXCEPT
{
    return x;
}

template <typename T, size_t N1, size_t N2>
KFR_INTRINSIC const simd<T, N1>& simd_shuffle(simd2_t<T, N1, N2>, const simd<T, N1>& x, const simd<T, N2>&,
                                              csizeseq_t<N1>, overload_priority<9>) CMT_NOEXCEPT
{
    return x;
}

template <typename T, size_t N1, size_t N2>
KFR_INTRINSIC const simd<T, N2>& simd_shuffle(simd2_t<T, N1, N2>, const simd<T, N1>&, const simd<T, N2>& y,
                                              csizeseq_t<N2, N1>, overload_priority<9>) CMT_NOEXCEPT
{
    return y;
}

// concat()
template <typename T, size_t N,
          KFR_ENABLE_IF(is_poweroftwo(N) && is_same<simd<T, N + N>, simd_halves<T, N + N>>::value)>
KFR_INTRINSIC simd<T, N + N> simd_shuffle(simd2_t<T, N, N>, const simd<T, N>& x, const simd<T, N>& y,
                                          csizeseq_t<N + N>, overload_priority<8>) CMT_NOEXCEPT
{
    return simd<T, N + N>{ x, y };
}

template <typename T>
KFR_INTRINSIC simd<T, 1> simd_broadcast(simd_t<T, 1>, identity<T> value) CMT_NOEXCEPT
{
    return { value };
}

template <typename T, size_t N, KFR_ENABLE_IF(N >= 2), size_t Nlow = prev_poweroftwo(N - 1)>
KFR_INTRINSIC simd<T, N> simd_broadcast(simd_t<T, N>, identity<T> value) CMT_NOEXCEPT
{
    return simd_concat<T, Nlow, N - Nlow>(simd_broadcast(simd_t<T, Nlow>{}, value),
                                          simd_broadcast(simd_t<T, N - Nlow>{}, value));
}

template <typename T, size_t N,
          KFR_ENABLE_IF(is_poweroftwo(N) && is_same<simd<T, N>, simd_halves<T, N>>::value)>
KFR_INTRINSIC simd<T, N / 2> simd_shuffle(simd_t<T, N>, const simd<T, N>& x, csizeseq_t<N / 2>,
                                          overload_priority<7>) CMT_NOEXCEPT
{
    return x.low;
}

template <typename T, size_t N,
          KFR_ENABLE_IF(is_poweroftwo(N) && is_same<simd<T, N>, simd_halves<T, N>>::value)>
KFR_INTRINSIC simd<T, N / 2> simd_shuffle(simd_t<T, N>, const simd<T, N>& x, csizeseq_t<N / 2, N / 2>,
                                          overload_priority<7>) CMT_NOEXCEPT
{
    return x.high;
}

template <typename T, size_t Nout, size_t N>
simd_array<T, Nout> simd_shuffle_generic(const simd_array<T, N>& x, const unsigned (&indices)[Nout])
{
    simd_array<T, Nout> result;
    for (size_t i = 0; i < Nout; ++i)
    {
        const size_t index = indices[i];
        result.val[i]      = index >= N ? T() : x.val[index];
    }
    return result;
}

template <typename T, size_t Nout, size_t N1, size_t N2>
simd_array<T, Nout> simd_shuffle2_generic(const simd_array<T, N1>& x, const simd_array<T, N2>& y,
                                          const unsigned (&indices)[Nout])
{
    simd_array<T, Nout> result;
    for (size_t i = 0; i < Nout; ++i)
    {
        const size_t index = indices[i];
        result.val[i]      = index > N1 + N2 ? T() : index >= N1 ? y.val[index - N1] : x.val[index];
    }
    return result;
}

template <typename T, size_t N, size_t... indices, size_t Nout = sizeof...(indices)>
KFR_INTRINSIC simd<T, Nout> simd_shuffle(simd_t<T, N>, const simd<T, N>& x, csizes_t<indices...>,
                                         overload_generic) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
#ifdef CMT_COMPILER_MSVC
    const simd_array<T, N> xx                 = to_simd_array<T, N>(x);
    constexpr static unsigned indices_array[] = { static_cast<unsigned>(indices)... };
    return from_simd_array<T, Nout>(simd_shuffle_generic<T, Nout, N>(xx, indices_array));
#else
    return from_simd_array<T, Nout>({ (indices > N ? T() : to_simd_array<T, N>(x).val[indices])... });
#endif
}

template <typename T, size_t N, size_t N2 = N, size_t... indices, size_t Nout = sizeof...(indices)>
KFR_INTRINSIC simd<T, Nout> simd_shuffle(simd2_t<T, N, N>, const simd<T, N>& x, const simd<T, N>& y,
                                         csizes_t<indices...>, overload_generic) CMT_NOEXCEPT
{
    static_assert(N == N2, "");
    not_optimized(CMT_FUNC_SIGNATURE);
#ifdef CMT_COMPILER_MSVC
    const simd_array<T, N> xx                 = to_simd_array<T, N>(x);
    const simd_array<T, N> yy                 = to_simd_array<T, N>(y);
    constexpr static unsigned indices_array[] = { static_cast<unsigned>(indices)... };
    return from_simd_array<T, Nout>(simd_shuffle2_generic<T, Nout, N, N>(xx, yy, indices_array));
#else
    return from_simd_array<T, Nout>(
        { (indices > N * 2 ? T()
                           : indices >= N ? to_simd_array<T, N>(y).val[indices - N]
                                          : to_simd_array<T, N>(x).val[indices])... });
#endif
}

template <typename T, size_t N1, size_t N2, size_t... indices, KFR_ENABLE_IF(N1 != N2),
          size_t Nout = sizeof...(indices)>
KFR_INTRINSIC simd<T, Nout> simd_shuffle(simd2_t<T, N1, N2>, const simd<T, N1>& x, const simd<T, N2>& y,
                                         csizes_t<indices...>, overload_generic) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);

#ifdef CMT_COMPILER_MSVC
    const simd_array<T, N1> xx                = to_simd_array<T, N1>(x);
    const simd_array<T, N2> yy                = to_simd_array<T, N2>(y);
    constexpr static unsigned indices_array[] = { static_cast<unsigned>(indices)... };
    return from_simd_array<T, Nout>(simd_shuffle2_generic<T, Nout, N1, N2>(xx, yy, indices_array));
#else

    return from_simd_array<T, Nout>(
        { (indices > N1 + N2 ? T()
                             : indices >= N1 ? to_simd_array<T, N2>(y).val[indices - N1]
                                             : to_simd_array<T, N1>(x).val[indices])... });
#endif
}

template <typename T, size_t N1>
KFR_INTRINSIC const simd<T, N1>& simd_concat(const simd<T, N1>& x) CMT_NOEXCEPT
{
    return x;
}

template <typename T, size_t N1, size_t N2, size_t... Ns, size_t Nscount /*= csum(csizes<Ns...>)*/>
KFR_INTRINSIC simd<T, N1 + N2 + Nscount> simd_concat(const simd<T, N1>& x, const simd<T, N2>& y,
                                                     const simd<T, Ns>&... z) CMT_NOEXCEPT
{
    return simd_shuffle(simd2_t<T, N1, N2 + Nscount>{}, x, simd_concat<T, N2, Ns...>(y, z...),
                        csizeseq<N1 + N2 + Nscount>, overload_auto);
}

template <typename Tout, typename Tin, size_t N, size_t... indices>
KFR_INTRINSIC simd<Tout, N> simd_convert__(const simd<Tin, N>& x, csizes_t<indices...>) CMT_NOEXCEPT
{
    const simd_array<Tin, N> xx = to_simd_array<Tin, N>(x);
    return simd_make(ctype<Tout>, static_cast<Tout>(xx.val[indices])...);
}

/// @brief Converts input vector to vector with subtype Tout
template <typename Tout, typename Tin>
KFR_INTRINSIC simd<Tout, 1> simd_convert(simd_cvt_t<Tout, Tin, 1>, const simd<Tin, 1>& x) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    return simd_make(ctype<Tout>, static_cast<Tout>(x));
}

/// @brief Converts input vector to vector with subtype Tout
template <typename Tout, typename Tin, size_t N>
KFR_INTRINSIC simd<Tout, N> simd_convert(simd_cvt_t<Tout, Tin, N>, const simd<Tin, N>& x) CMT_NOEXCEPT
{
    constexpr size_t Nlow = prev_poweroftwo(N - 1);
    return simd_concat<Tout, Nlow, N - Nlow>(
        simd_convert(simd_cvt_t<Tout, Tin, Nlow>{},
                     simd_shuffle(simd_t<Tin, N>{}, x, csizeseq<Nlow>, overload_auto)),
        simd_convert(simd_cvt_t<Tout, Tin, N - Nlow>{},
                     simd_shuffle(simd_t<Tin, N>{}, x, csizeseq<N - Nlow, Nlow>, overload_auto)));
}

/// @brief Converts input vector to vector with subtype Tout
template <typename T, size_t N>
KFR_INTRINSIC const simd<T, N>& simd_convert(simd_cvt_t<T, T, N>, const simd<T, N>& x) CMT_NOEXCEPT
{
    return x;
}

CMT_PRAGMA_GNU(GCC diagnostic push)
CMT_PRAGMA_GNU(GCC diagnostic ignored "-Wignored-attributes")

template <typename T, size_t N, bool A>
using simd_storage = struct_with_alignment<simd<T, N>, A>;

CMT_PRAGMA_GNU(GCC diagnostic pop)

template <size_t N, bool A = false, typename T, KFR_ENABLE_IF(is_poweroftwo(N))>
KFR_INTRINSIC simd<T, N> simd_read(const T* src) CMT_NOEXCEPT
{
    return reinterpret_cast<typename simd_storage<T, N, A>::const_pointer>(src)->value;
}

template <size_t N, bool A = false, typename T, KFR_ENABLE_IF(!is_poweroftwo(N)), typename = void>
KFR_INTRINSIC simd<T, N> simd_read(const T* src) CMT_NOEXCEPT
{
    constexpr size_t first = prev_poweroftwo(N);
    constexpr size_t rest  = N - first;
    constexpr auto extend_indices =
        cconcat(csizeseq_t<rest>(), csizeseq_t<first - rest, index_undefined, 0>());
    constexpr auto concat_indices = cvalseq_t<size_t, N>();
    return simd_shuffle(
        simd2_t<T, first, first>{}, simd_read<first, A>(src),
        simd_shuffle(simd_t<T, rest>{}, simd_read<rest, false>(src + first), extend_indices, overload_auto),
        concat_indices, overload_auto);
}

template <bool A = false, size_t N, typename T, KFR_ENABLE_IF(is_poweroftwo(N))>
KFR_INTRINSIC void simd_write(T* dest, const simd<T, N>& value) CMT_NOEXCEPT
{
    reinterpret_cast<typename simd_storage<T, N, A>::pointer>(dest)->value = value;
}

template <bool A = false, size_t N, typename T, KFR_ENABLE_IF(!is_poweroftwo(N)), typename = void>
KFR_INTRINSIC void simd_write(T* dest, const simd<T, N>& value) CMT_NOEXCEPT
{
    constexpr size_t first = prev_poweroftwo(N);
    constexpr size_t rest  = N - first;
    simd_write<A, first>(dest, simd_shuffle(simd_t<T, N>{}, value, csizeseq_t<first>(), overload_auto));
    simd_write<false, rest>(dest + first,
                            simd_shuffle(simd_t<T, N>{}, value, csizeseq_t<rest, first>(), overload_auto));
}

template <typename T, size_t N>
KFR_INTRINSIC T simd_get_element(const simd<T, N>& value, size_t index) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    return to_simd_array<T, N>(value).val[index];
}

template <typename T, size_t N>
KFR_INTRINSIC simd<T, N> simd_set_element(const simd<T, N>& value, size_t index, T x) CMT_NOEXCEPT
{
    not_optimized(CMT_FUNC_SIGNATURE);
    simd_array<T, N> arr = to_simd_array<T, N>(value);
    arr.val[index]       = x;
    return from_simd_array(arr);
}
} // namespace intrinsics
} // namespace CMT_ARCH_NAME
} // namespace kfr

CMT_PRAGMA_GNU(GCC diagnostic pop)