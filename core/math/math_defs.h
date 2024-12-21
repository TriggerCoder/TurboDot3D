/**************************************************************************/
/*  math_defs.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-2024 Godot Engine contributors (see ORGAUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef MATH_DEFS_H
#define MATH_DEFS_H


#if defined(_MSC_VER)

	#pragma warning(push)
	#pragma warning(disable: 4530)		// C++ exception handler used, but unwind semantics are not enabled
	#pragma warning(disable: 4577)		// 'noexcept' used with no exception handling mode specified

#endif

#include <new>
#include <string.h>

#if defined(_MSC_VER)

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef __int64 int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

#if defined(_M_X64) || defined(_M_ARM64)

typedef __int64 machine;
typedef unsigned __int64 umachine;
typedef unsigned __int64 machine_address;

#else

typedef int machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;

#endif

#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4244) // conversion, possible loss of data
#pragma warning(disable : 4324) // structure was padded due to alignment specifier
#pragma warning(disable : 4458) // declaration hides class member
#pragma warning(disable : 4522) // multiple assignment operators specified

#elif defined(__ORBIS__) || defined(__PROSPERO__)

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef long machine;
typedef unsigned long umachine;
typedef unsigned long machine_address;

#elif defined(__GNUC__)

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#if defined(__LP64__) || defined(_WIN64)

typedef long long machine;
typedef unsigned long long umachine;
typedef unsigned long long machine_address;

#else

typedef int machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;

#endif

#endif

inline const float &asfloat(const int32_t &i) {
	return (reinterpret_cast<const float &>(i));
}

inline const float &asfloat(const uint32_t &i) {
	return (reinterpret_cast<const float &>(i));
}

inline const int32_t &asint(const float &f) {
	return (reinterpret_cast<const int32_t &>(f));
}

inline const uint32_t &asuint(const float &f) {
	return (reinterpret_cast<const uint32_t &>(f));
}

#define CMP_EPSILON 0.00001
#define CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)

#define CMP_NORMALIZE_TOLERANCE 0.000001
#define CMP_POINT_IN_PLANE_EPSILON 0.00001

//Shaders Constants
#define Math_TAU "6.2831853071795864769252867666"
#define Math_PI "3.1415926535897932384626433833"
#define Math_E "2.7182818284590452353602874714"

constexpr float Math_sqrt_2_over_2 = 0.70710678118654752440084436210485F;
constexpr float Math_sqrt_2 = 1.4142135623730950488016887242097F;

constexpr float Math_euler = 2.718281828459045235360287471353F;
constexpr float Math_two_tau = 12.566370614359172953850573533118F;
constexpr float Math_tau = 6.2831853071795864769252867665590F;
constexpr float Math_tau_over_2 = 3.1415926535897932384626433832795F;
constexpr float Math_three_tau_over_8 = 2.3561944901923449288469825374596F;
constexpr float Math_tau_over_3 = 2.0943951023931954923084289221863F;
constexpr float Math_tau_over_4 = 1.5707963267948966192313216916398F;
constexpr float Math_tau_over_8 = 0.78539816339744830961566084581988F;
constexpr float Math_tau_over_12 = 0.52359877559829887307710723054658F;
constexpr float Math_tau_over_24 = 0.26179938779914943653855361527329F;

constexpr float Math_ln_2 = 0.69314718055994530941723212145818F;
constexpr float Math_infinity = 0x7F800000;
constexpr float Math_minus_infinity = 0xFF800000;
constexpr float Math_min_float = 0x00800000;
constexpr float Math_max_float = 0x7F7FFFFF;


#ifdef DEBUG_ENABLED
#define MATH_CHECKS
#endif

//this epsilon is for values related to a unit size (scalar or vector len)
#ifdef PRECISE_MATH_CHECKS
#define UNIT_EPSILON 0.00001
#else
//tolerate some more floating point error normally
#define UNIT_EPSILON 0.001
#endif

#define USEC_TO_SEC(m_usec) ((m_usec) / 1000000.0)

enum ClockDirection {
	CLOCKWISE,
	COUNTERCLOCKWISE
};

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

enum HorizontalAlignment {
	HORIZONTAL_ALIGNMENT_LEFT,
	HORIZONTAL_ALIGNMENT_CENTER,
	HORIZONTAL_ALIGNMENT_RIGHT,
	HORIZONTAL_ALIGNMENT_FILL,
};

enum VerticalAlignment {
	VERTICAL_ALIGNMENT_TOP,
	VERTICAL_ALIGNMENT_CENTER,
	VERTICAL_ALIGNMENT_BOTTOM,
	VERTICAL_ALIGNMENT_FILL,
};

enum InlineAlignment {
	// Image alignment points.
	INLINE_ALIGNMENT_TOP_TO = 0b0000,
	INLINE_ALIGNMENT_CENTER_TO = 0b0001,
	INLINE_ALIGNMENT_BASELINE_TO = 0b0011,
	INLINE_ALIGNMENT_BOTTOM_TO = 0b0010,
	INLINE_ALIGNMENT_IMAGE_MASK = 0b0011,

	// Text alignment points.
	INLINE_ALIGNMENT_TO_TOP = 0b0000,
	INLINE_ALIGNMENT_TO_CENTER = 0b0100,
	INLINE_ALIGNMENT_TO_BASELINE = 0b1000,
	INLINE_ALIGNMENT_TO_BOTTOM = 0b1100,
	INLINE_ALIGNMENT_TEXT_MASK = 0b1100,

	// Presets.
	INLINE_ALIGNMENT_TOP = INLINE_ALIGNMENT_TOP_TO | INLINE_ALIGNMENT_TO_TOP,
	INLINE_ALIGNMENT_CENTER = INLINE_ALIGNMENT_CENTER_TO | INLINE_ALIGNMENT_TO_CENTER,
	INLINE_ALIGNMENT_BOTTOM = INLINE_ALIGNMENT_BOTTOM_TO | INLINE_ALIGNMENT_TO_BOTTOM
};

enum Side {
	SIDE_LEFT,
	SIDE_TOP,
	SIDE_RIGHT,
	SIDE_BOTTOM
};

enum Corner {
	CORNER_TOP_LEFT,
	CORNER_TOP_RIGHT,
	CORNER_BOTTOM_RIGHT,
	CORNER_BOTTOM_LEFT
};

enum class EulerOrder {
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

/**
 * The "Real" type is an abstract type used for real numbers, such as 1.5,
 * in contrast to integer numbers. Precision can be controlled with the
 * presence or absence of the REAL_T_IS_DOUBLE define.
 */
#ifdef REAL_T_IS_DOUBLE
typedef double real_t;
#else
typedef float real_t;
#endif

/**
 * SIMD
 *
 */
#if ((defined(_M_X64) || defined(_M_IX86) || defined(__x86_64__) || defined(__SSE__)) && !(defined(SSE_ENABLED)))
#define SSE_ENABLED
#endif
#if (defined(__AVX__) && !(defined(AVX_ENABLED)))
#define AVX_ENABLED
#endif
#if (defined(__ARM_NEON) && !(defined(NEON_ENABLED)))
#define NEON_ENABLED
#endif

#if defined(SSE_ENABLED) || defined(AVX_ENABLED) || defined(NEON_ENABLED)
#define SIMD_ENABLED
#endif

#ifdef SIMD_ENABLED

#ifdef SSE_ENABLED
#ifdef REAL_T_IS_DOUBLE
#define SSE_WITH_DOUBLE_PRECISION
#else // REAL_T_IS_DOUBLE
#define SSE_WITH_SINGLE_PRECISION
#endif // REAL_T_IS_DOUBLE
#endif // SSE_ENABLED

#ifdef NEON_ENABLED
#ifdef REAL_T_IS_DOUBLE
#define NEON_WITH_DOUBLE_PRECISION
#else // REAL_T_IS_DOUBLE
#define NEON_WITH_SINGLE_PRECISION
#endif // REAL_T_IS_DOUBLE
#endif // NEON_ENABLED

// Include required headers.
#if defined(SSE_ENABLED)
#include <emmintrin.h>
	#if defined(AVX_ENABLED)
		#include <immintrin.h>
	#endif // AVX_ENABLED
#elif defined(NEON_ENABLED)
#ifdef _MSC_VER
#include <arm64_neon.h>
#include <intrin.h>
#else // _MSC_VER
#include <arm_neon.h>
#endif // _MSC_VER
#endif
#endif // SIMD_ENABLED

struct vec_float {
#if defined(SSE_ENABLED)

	__m128 v;

	vec_float(__m128 m) { v = m; }

	operator __m128 &(void) { return (v); }
	operator const __m128 &(void) const { return (v); }
	operator volatile __m128 &(void) volatile { return (v); }
	operator const volatile __m128 &(void) const volatile { return (v); }

#elif defined(NEON_ENABLED)

	float32x4_t v;

	vec_float(float32x4_t m) { v = m; }

	operator float32x4_t &(void) { return (v); }
	operator const float32x4_t &(void) const { return (v); }
	operator volatile float32x4_t &(void) volatile { return (v); }
	operator const volatile float32x4_t &(void) const volatile { return (v); }

#else

	float v[4];

#endif

	inline vec_float() = default;
	inline vec_float(const vec_float &) = default;
	inline vec_float &operator=(const vec_float &m) = default;
};

struct exv_float {
#if defined(AVX_ENABLED)

	__m256 v;

	exv_float(__m256 m) { v = m; }

	operator __m256 &(void) { return (v); }
	operator const __m256 &(void) const { return (v); }
	operator volatile __m256 &(void) volatile { return (v); }
	operator const volatile __m256 &(void) const volatile { return (v); }

#else

	float v[8];

#endif

	inline exv_float() = default;
	inline exv_float(const exv_float &) = default;
	inline exv_float &operator=(const exv_float &) = default;
};

#if defined(SSE_ENABLED)

typedef __m128i vec_int8;
typedef __m128i vec_int16;
typedef __m128i vec_int32;
typedef __m128i vec_uint8;
typedef __m128i vec_uint16;
typedef __m128i vec_uint32;

#elif defined(NEON_ENABLED)

typedef int8x16_t vec_int8;
typedef int16x8_t vec_int16;
typedef int32x4_t vec_int32;
typedef uint8x16_t vec_uint8;
typedef uint16x8_t vec_uint16;
typedef uint32x4_t vec_uint32;

#endif

#if defined(AVX_ENABLED)

typedef __m256i exv_int8;
typedef __m256i exv_int16;
typedef __m256i exv_int32;
typedef __m256i exv_uint8;
typedef __m256i exv_uint16;
typedef __m256i exv_uint32;

#endif

inline void SimdInit(void) {
#if defined(SSE_ENABLED)

	_mm_setcsr(_mm_getcsr() | 0x8040);

#endif
}

inline vec_float VecFloatGetZero(void) {
#if defined(SSE_ENABLED)

	return (_mm_setzero_ps());

#elif defined(NEON_ENABLED)

	return (vmovq_n_f32(0.0F));

#endif
}

inline vec_float VecFloatGetMinusZero(void) {
#if defined(SSE_ENABLED)

	alignas(16) static const uint32_t float_80000000[4] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
	return (_mm_load_ps(reinterpret_cast<const float *>(float_80000000)));

#elif defined(NEON_ENABLED)

	return (vmovq_n_f32(-0.0F));

#endif
}

template <uint32_t value>
inline vec_float VecLoadVectorConstant(void) {
#if defined(SSE_ENABLED)

	alignas(16) static const uint32_t k[4] = { value, value, value, value };
	return (_mm_load_ps(reinterpret_cast<const float *>(k)));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vdupq_n_u32(value)));

#endif
}

template <uint32_t value>
inline vec_float VecLoadScalarConstant(void) {
#if defined(SSE_ENABLED)

	static const uint32_t k = value;
	return (_mm_load_ss(reinterpret_cast<const float *>(&k)));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vdupq_n_u32(value)));

#endif
}

inline vec_float VecSmearX(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));

#elif defined(NEON_ENABLED)

	return (vdupq_n_f32(vgetq_lane_f32(v, 0)));

#endif
}

inline vec_float VecSmearY(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));

#elif defined(NEON_ENABLED)

	return (vdupq_n_f32(vgetq_lane_f32(v, 1)));

#endif
}

inline vec_float VecSmearZ(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));

#elif defined(NEON_ENABLED)

	return (vdupq_n_f32(vgetq_lane_f32(v, 2)));

#endif
}

inline vec_float VecSmearW(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));

#elif defined(NEON_ENABLED)

	return (vdupq_n_f32(vgetq_lane_f32(v, 3)));

#endif
}

#if defined(SSE_ENABLED)

template <int p3, int p2, int p1, int p0>
inline vec_float VecShuffle(const vec_float &v1, const vec_float &v2) {
	return (_mm_shuffle_ps(v1, v2, _MM_SHUFFLE(p3, p2, p1, p0)));
}

#endif

inline vec_float VecLoad(const float *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_load_ps(ptr));

#elif defined(NEON_ENABLED)

	return (vld1q_f32(ptr));

#endif
}

inline vec_float VecLoadUnaligned(const float *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_loadu_ps(ptr));

#elif defined(NEON_ENABLED)

	return (vld1q_f32(ptr));

#endif
}

inline vec_float VecLoadScalar(const float *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_load_ss(ptr));

#elif defined(NEON_ENABLED)

	return (vld1q_dup_f32(ptr));

#endif
}

inline vec_float VecLoadSmearScalar(const float *ptr) {
#if defined(SSE_ENABLED)

	vec_float v = _mm_load_ss(ptr);
	return (_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));

#elif defined(NEON_ENABLED)

	return (vld1q_dup_f32(ptr));

#endif
}

inline void VecStore(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED)

	_mm_store_ps(ptr, v);

#elif defined(NEON_ENABLED)

	vst1q_f32(ptr, v);

#endif
}

inline void VecStoreUnaligned(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED)

	_mm_storeu_ps(ptr, v);

#elif defined(NEON_ENABLED)

	vst1q_f32(ptr, v);

#endif
}

inline void VecStoreX(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED4)

	*ptr = v[0];

#elif defined(SSE_ENABLED)

	_mm_store_ss(ptr, v);

#elif defined(NEON_ENABLED)

	vst1q_lane_f32(ptr, v, 0);

#endif
}

inline void VecStoreY(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED4)

	*ptr = v[1];

#elif defined(SSE_ENABLED)

	_mm_store_ss(ptr, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));

#elif defined(NEON_ENABLED)

	vst1q_lane_f32(ptr, v, 1);

#endif
}

inline void VecStoreZ(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED4)

	*ptr = v[2];

#elif defined(SSE_ENABLED)

	_mm_store_ss(ptr, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));

#elif defined(NEON_ENABLED)

	vst1q_lane_f32(ptr, v, 2);

#endif
}

inline void VecStoreW(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED4)

	*ptr = v[3];

#elif defined(SSE_ENABLED)

	_mm_store_ss(ptr, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));

#elif defined(NEON_ENABLED)

	vst1q_lane_f32(ptr, v, 3);

#endif
}

inline void VecStore3D(const vec_float &v, float *ptr) {
#if defined(SSE_ENABLED4)

	ptr[0] = v[0];
	ptr[1] = v[1];
	ptr[2] = v[2];

#elif defined(SSE_ENABLED)

	_mm_store_ss(&ptr[0], v);
	_mm_store_ss(&ptr[1], _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
	_mm_store_ss(&ptr[2], _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));

#elif defined(NEON_ENABLED)

	vst1q_lane_f32(&ptr[0], v, 0);
	vst1q_lane_f32(&ptr[1], v, 1);
	vst1q_lane_f32(&ptr[2], v, 2);

#endif
}

inline int32_t VecTruncateConvert(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_cvtt_ss2si(v));

#elif defined(NEON_ENABLED)

	return (vgetq_lane_s32(vcvtq_s32_f32(v), 0));

#endif
}

inline vec_float VecNegate(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ps(_mm_setzero_ps(), v));

#elif defined(NEON_ENABLED)

	return (vnegq_f32(v));

#endif
}

inline vec_float operator-(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ps(_mm_setzero_ps(), v));

#elif defined(NEON_ENABLED)

	return (vnegq_f32(v));

#endif
}

inline vec_float VecMin(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_min_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vminq_f32(v1, v2));

#endif
}

inline vec_float VecMinScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_min_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vminq_f32(v1, v2));

#endif
}

inline vec_float VecMax(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_max_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vmaxq_f32(v1, v2));

#endif
}

inline vec_float VecMaxScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_max_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vmaxq_f32(v1, v2));

#endif
}

inline vec_float VecAdd(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_add_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vaddq_f32(v1, v2));

#endif
}

inline vec_float operator+(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_add_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vaddq_f32(v1, v2));

#endif
}

inline vec_float VecAddScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_add_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vaddq_f32(v1, v2));

#endif
}

inline vec_float VecSub(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vsubq_f32(v1, v2));

#endif
}

inline vec_float operator-(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vsubq_f32(v1, v2));

#endif
}

inline vec_float VecSubScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vsubq_f32(v1, v2));

#endif
}

inline vec_float VecMul(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_mul_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vmulq_f32(v1, v2));

#endif
}

inline vec_float operator*(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_mul_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vmulq_f32(v1, v2));

#endif
}

inline vec_float VecMulScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_mul_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vmulq_f32(v1, v2));

#endif
}

inline vec_float VecMadd(const vec_float &v1, const vec_float &v2, const vec_float &v3) {
#if defined(SSE_ENABLED)

	return (_mm_add_ps(_mm_mul_ps(v1, v2), v3));

#elif defined(NEON_ENABLED)

	return (vfmaq_f32(v3, v1, v2));

#endif
}

inline vec_float VecMaddScalar(const vec_float &v1, const vec_float &v2, const vec_float &v3) {
#if defined(SSE_ENABLED)

	return (_mm_add_ss(_mm_mul_ss(v1, v2), v3));

#elif defined(NEON_ENABLED)

	return (vfmaq_f32(v3, v1, v2));

#endif
}

inline vec_float VecNmsub(const vec_float &v1, const vec_float &v2, const vec_float &v3) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ps(v3, _mm_mul_ps(v1, v2)));

#elif defined(NEON_ENABLED)

	return (vfmsq_f32(v3, v1, v2));

#endif
}

inline vec_float VecNmsubScalar(const vec_float &v1, const vec_float &v2, const vec_float &v3) {
#if defined(SSE_ENABLED)

	return (_mm_sub_ss(v3, _mm_mul_ss(v1, v2)));

#elif defined(NEON_ENABLED)

	return (vfmsq_f32(v3, v1, v2));

#endif
}

inline vec_float VecDiv(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_div_ps(v1, v2));

#elif defined(NEON_ENABLED)

	vec_float f = vrecpeq_f32(v2);
	f = vmulq_f32(f, vrecpsq_f32(v2, f));
	return (vmulq_f32(v1, f));

#endif
}

inline vec_float operator/(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_div_ps(v1, v2));

#elif defined(NEON_ENABLED)

	vec_float f = vrecpeq_f32(v2);
	f = vmulq_f32(f, vrecpsq_f32(v2, f));
	return (vmulq_f32(v1, f));

#endif
}

inline vec_float VecDivScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_div_ss(v1, v2));

#elif defined(NEON_ENABLED)

	vec_float f = vrecpeq_f32(v2);
	f = vmulq_f32(f, vrecpsq_f32(v2, f));
	return (vmulq_f32(v1, f));

#endif
}

inline vec_float VecAnd(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_and_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float operator&(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_and_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float VecAndc(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_andnot_ps(v2, v1));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vbicq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float VecOr(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_or_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float operator|(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_or_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float VecXor(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_xor_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float operator^(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_xor_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v1), vreinterpretq_u32_f32(v2))));

#endif
}

inline vec_float VecSelect(const vec_float &v1, const vec_float &v2, const vec_float &mask) {
#if defined(SSE_ENABLED)

	return (_mm_or_ps(_mm_andnot_ps(mask, v1), _mm_and_ps(mask, v2)));

#elif defined(NEON_ENABLED)

	return (vbslq_f32(vreinterpretq_u32_f32(mask), v2, v1));

#endif
}

inline vec_float VecMaskCmpeq(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_cmpeq_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vceqq_f32(v1, v2)));

#endif
}

inline vec_float VecMaskCmplt(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_cmplt_ps(v1, v2));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vcltq_f32(v1, v2)));

#endif
}

inline vec_float VecMaskCmpgt(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_cmplt_ps(v2, v1));

#elif defined(NEON_ENABLED)

	return (vreinterpretq_f32_u32(vcgtq_f32(v1, v2)));

#endif
}

inline bool VecCmpeqScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_comieq_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vgetq_lane_f32(v1, 0) == vgetq_lane_f32(v2, 0));

#endif
}

inline bool VecCmpltScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_comilt_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vgetq_lane_f32(v1, 0) < vgetq_lane_f32(v2, 0));

#endif
}

inline bool VecCmpgtScalar(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return (_mm_comigt_ss(v1, v2));

#elif defined(NEON_ENABLED)

	return (vgetq_lane_f32(v1, 0) > vgetq_lane_f32(v2, 0));

#endif
}

inline bool VecCmpltAny3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return ((_mm_comilt_ss(v1, v2) | _mm_comilt_ss(VecSmearY(v1), VecSmearY(v2)) | _mm_comilt_ss(VecSmearZ(v1), VecSmearZ(v2))) != 0);

#elif defined(NEON_ENABLED)

	return ((vgetq_lane_f32(v1, 0) < vgetq_lane_f32(v2, 0)) || (vgetq_lane_f32(v1, 1) < vgetq_lane_f32(v2, 1)) || (vgetq_lane_f32(v1, 2) < vgetq_lane_f32(v2, 2)));

#endif
}

inline bool VecCmpgtAny3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	return ((_mm_comigt_ss(v1, v2) | _mm_comigt_ss(VecSmearY(v1), VecSmearY(v2)) | _mm_comigt_ss(VecSmearZ(v1), VecSmearZ(v2))) != 0);

#elif defined(NEON_ENABLED)

	return ((vgetq_lane_f32(v1, 0) > vgetq_lane_f32(v2, 0)) || (vgetq_lane_f32(v1, 1) > vgetq_lane_f32(v2, 1)) || (vgetq_lane_f32(v1, 2) > vgetq_lane_f32(v2, 2)));

#endif
}

inline vec_float VecInverseSqrt(const vec_float &v) {
#if defined(SSE_ENABLED)

	const vec_float three = VecLoadVectorConstant<0x40400000>();
	const vec_float half = VecLoadVectorConstant<0x3F000000>();

	vec_float f = _mm_rsqrt_ps(v);
	return (_mm_mul_ps(_mm_mul_ps(_mm_sub_ps(three, _mm_mul_ps(v, _mm_mul_ps(f, f))), f), half));

#elif defined(NEON_ENABLED)

	vec_float f = vrsqrteq_f32(v);
	return (vmulq_f32(f, vrsqrtsq_f32(v, vmulq_f32(f, f))));

#endif
}

inline vec_float VecInverseSqrtScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

	const vec_float three = VecLoadVectorConstant<0x40400000>();
	const vec_float half = VecLoadVectorConstant<0x3F000000>();

	vec_float f = _mm_rsqrt_ss(v);
	return (_mm_mul_ss(_mm_mul_ss(_mm_sub_ss(three, _mm_mul_ss(v, _mm_mul_ss(f, f))), f), half));

#elif defined(NEON_ENABLED)

	vec_float f = vrsqrteq_f32(v);
	return (vmulq_f32(f, vrsqrtsq_f32(v, vmulq_f32(f, f))));

#endif
}

inline vec_float VecSqrt(const vec_float &v) {
	vec_float mask = VecMaskCmpeq(v, VecFloatGetZero());
	return (VecAndc(VecMul(VecInverseSqrt(v), v), mask));
}

inline vec_float VecSqrtScalar(const vec_float &v) {
	vec_float mask = VecMaskCmpeq(v, VecFloatGetZero());
	return (VecAndc(VecMulScalar(VecInverseSqrtScalar(v), v), mask));
}

inline vec_float VecFloor(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ps(_mm_add_ps(_mm_add_ps(_mm_sub_ps(v, two23), two23), two23), two23);
	result = _mm_sub_ps(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));

	vec_float mask = _mm_cmplt_ps(two23, _mm_andnot_ps(VecFloatGetMinusZero(), v));
	return (_mm_or_ps(_mm_andnot_ps(mask, result), _mm_and_ps(mask, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(vaddq_f32(vsubq_f32(v, two23), two23), two23), two23);
	result = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));

	vec_uint32 mask = vcltq_f32(two23, vabsq_f32(v));
	return (vbslq_f32(mask, v, result));

#endif
}

inline vec_float VecFloorScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ss(_mm_add_ss(_mm_add_ss(_mm_sub_ss(v, two23), two23), two23), two23);
	result = _mm_sub_ss(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));

	vec_float mask = _mm_cmplt_ps(two23, _mm_andnot_ps(VecFloatGetMinusZero(), v));
	return (_mm_or_ps(_mm_andnot_ps(mask, result), _mm_and_ps(mask, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(vaddq_f32(vsubq_f32(v, two23), two23), two23), two23);
	result = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));

	vec_uint32 mask = vcltq_f32(two23, vabsq_f32(v));
	return (vbslq_f32(mask, v, result));

#endif
}

inline vec_float VecPositiveFloor(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ps(_mm_add_ps(v, two23), two23);
	return (_mm_sub_ps(result, _mm_and_ps(one, _mm_cmplt_ps(v, result))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(v, two23), two23);
	return (vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result)))));

#endif
}

inline vec_float VecPositiveFloorScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ss(_mm_add_ss(v, two23), two23);
	return (_mm_sub_ss(result, _mm_and_ps(one, _mm_cmplt_ps(v, result))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(v, two23), two23);
	return (vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result)))));

#endif
}

inline vec_float VecNegativeFloor(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_add_ps(_mm_sub_ps(v, two23), two23);
	return (_mm_sub_ps(result, _mm_and_ps(one, _mm_cmplt_ps(v, result))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vaddq_f32(vsubq_f32(v, two23), two23);
	return (vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result)))));

#endif
}

inline vec_float VecNegativeFloorScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_add_ss(_mm_sub_ss(v, two23), two23);
	return (_mm_sub_ss(result, _mm_and_ps(one, _mm_cmplt_ps(v, result))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vaddq_f32(vsubq_f32(v, two23), two23);
	return (vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result)))));

#endif
}

inline vec_float VecCeil(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ps(_mm_add_ps(_mm_add_ps(_mm_sub_ps(v, two23), two23), two23), two23);
	result = _mm_add_ps(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

	vec_float mask = _mm_cmplt_ps(two23, _mm_andnot_ps(VecFloatGetMinusZero(), v));
	return (_mm_or_ps(_mm_andnot_ps(mask, result), _mm_and_ps(mask, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(vaddq_f32(vsubq_f32(v, two23), two23), two23), two23);
	result = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

	vec_uint32 mask = vcltq_f32(two23, vabsq_f32(v));
	return (vbslq_f32(mask, v, result));

#endif
}

inline vec_float VecCeilScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ss(_mm_add_ss(_mm_add_ss(_mm_sub_ss(v, two23), two23), two23), two23);
	result = _mm_add_ss(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

	vec_float mask = _mm_cmplt_ps(two23, _mm_andnot_ps(VecFloatGetMinusZero(), v));
	return (_mm_or_ps(_mm_andnot_ps(mask, result), _mm_and_ps(mask, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(vaddq_f32(vsubq_f32(v, two23), two23), two23), two23);
	result = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

	vec_uint32 mask = vcltq_f32(two23, vabsq_f32(v));
	return (vbslq_f32(mask, v, result));

#endif
}

inline vec_float VecPositiveCeil(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ps(_mm_add_ps(v, two23), two23);
	return (_mm_add_ps(result, _mm_and_ps(one, _mm_cmplt_ps(result, v))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(v, two23), two23);
	return (vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v)))));

#endif
}

inline vec_float VecPositiveCeilScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ss(_mm_add_ss(v, two23), two23);
	return (_mm_add_ss(result, _mm_and_ps(one, _mm_cmplt_ps(result, v))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(v, two23), two23);
	return (vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v)))));

#endif
}

inline vec_float VecNegativeCeil(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_add_ps(_mm_sub_ps(v, two23), two23);
	return (_mm_add_ps(result, _mm_and_ps(one, _mm_cmplt_ps(result, v))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vaddq_f32(vsubq_f32(v, two23), two23);
	return (vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v)))));

#endif
}

inline vec_float VecNegativeCeilScalar(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_add_ss(_mm_sub_ss(v, two23), two23);
	return (_mm_add_ss(result, _mm_and_ps(one, _mm_cmplt_ps(result, v))));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vaddq_f32(vsubq_f32(v, two23), two23);
	return (vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v)))));

#endif
}

inline void VecFloorCeil(const vec_float &v, vec_float *f, vec_float *c) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	*f = _mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	*c = _mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ps(_mm_add_ps(_mm_add_ps(_mm_sub_ps(v, two23), two23), two23), two23);
	*f = _mm_sub_ps(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));
	*c = _mm_add_ps(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(vaddq_f32(vsubq_f32(v, two23), two23), two23), two23);
	*f = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));
	*c = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

#endif
}

inline void VecFloorCeilScalar(const vec_float &v, vec_float *f, vec_float *c) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	*f = _mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	*c = _mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ss(_mm_add_ss(_mm_add_ss(_mm_sub_ss(v, two23), two23), two23), two23);
	*f = _mm_sub_ss(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));
	*c = _mm_add_ss(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(vaddq_f32(vsubq_f32(v, two23), two23), two23), two23);
	*f = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));
	*c = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

#endif
}

inline void VecPositiveFloorCeil(const vec_float &v, vec_float *f, vec_float *c) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	*f = _mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	*c = _mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ps(_mm_add_ps(v, two23), two23);
	*f = _mm_sub_ps(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));
	*c = _mm_add_ps(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(v, two23), two23);
	*f = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));
	*c = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

#endif
}

inline void VecPositiveFloorCeilScalar(const vec_float &v, vec_float *f, vec_float *c) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	*f = _mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	*c = _mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_sub_ss(_mm_add_ss(v, two23), two23);
	*f = _mm_sub_ss(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));
	*c = _mm_add_ss(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vsubq_f32(vaddq_f32(v, two23), two23);
	*f = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));
	*c = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

#endif
}

inline void VecNegativeFloorCeil(const vec_float &v, vec_float *f, vec_float *c) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	*f = _mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	*c = _mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_add_ps(_mm_sub_ps(v, two23), two23);
	*f = _mm_sub_ps(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));
	*c = _mm_add_ps(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vaddq_f32(vsubq_f32(v, two23), two23);
	*f = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));
	*c = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

#endif
}

inline void VecNegativeFloorCeilScalar(const vec_float &v, vec_float *f, vec_float *c) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	*f = _mm_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	*c = _mm_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);

#else

	const vec_float one = VecLoadVectorConstant<0x3F800000>();
	const vec_float two23 = VecLoadVectorConstant<0x4B000000>();

	vec_float result = _mm_add_ss(_mm_sub_ss(v, two23), two23);
	*f = _mm_sub_ss(result, _mm_and_ps(one, _mm_cmplt_ps(v, result)));
	*c = _mm_add_ss(result, _mm_and_ps(one, _mm_cmplt_ps(result, v)));

#endif

#elif defined(NEON_ENABLED)

	const vec_uint32 one = vdupq_n_u32(0x3F800000);
	const vec_float two23 = vdupq_n_f32(8388608.0F);

	vec_float result = vaddq_f32(vsubq_f32(v, two23), two23);
	*f = vsubq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(v, result))));
	*c = vaddq_f32(result, vreinterpretq_f32_u32(vandq_u32(one, vcltq_f32(result, v))));

#endif
}

inline vec_float VecFsgn(const vec_float &v) {
	vec_float result = VecLoadVectorConstant<0x3F800000>();
	result = result | (VecFloatGetMinusZero() & v);
	return (VecAndc(result, VecMaskCmpeq(v, VecFloatGetZero())));
}

inline vec_float VecFnsgn(const vec_float &v) {
	vec_float result = VecLoadVectorConstant<0x3F800000>();
	result = result | VecAndc(VecFloatGetMinusZero(), v);
	return (VecAndc(result, VecMaskCmpeq(v, VecFloatGetZero())));
}

inline vec_float VecNonzeroFsgn(const vec_float &v) {
	vec_float result = VecLoadVectorConstant<0x3F800000>();
	return (result | (VecFloatGetMinusZero() & v));
}

inline vec_float VecNonzeroFnsgn(const vec_float &v) {
	vec_float result = VecLoadVectorConstant<0x3F800000>();
	return (result | VecAndc(VecFloatGetMinusZero(), v));
}

inline vec_float VecHorizontalSum3D(const vec_float &v) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	return (_mm_add_ss(_mm_hadd_ps(v, v), _mm_insert_ps(v, v, 0x8E)));

#else

	return (_mm_add_ss(_mm_add_ss(v, VecSmearY(v)), VecSmearZ(v)));

#endif

#elif defined(NEON_ENABLED)

	return (vaddq_f32(vpaddq_f32(v, v), vdupq_n_f32(vgetq_lane_f32(v, 2))));

#endif
}

inline vec_float VecDot3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	vec_float r = _mm_mul_ps(v1, v2);
	r = _mm_insert_ps(r, r, 0x08);
	r = _mm_hadd_ps(r, r);
	return (_mm_hadd_ps(r, r));

#else

	vec_float d = _mm_mul_ps(v1, v2);
	return (_mm_add_ss(_mm_add_ss(d, VecSmearY(d)), VecSmearZ(d)));

#endif

#elif defined(NEON_ENABLED)

	vec_float d = vmulq_f32(v1, v2);
	return (vaddq_f32(vpaddq_f32(d, d), vdupq_n_f32(vgetq_lane_f32(d, 2))));

#endif
}

inline vec_float VecDot4D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	vec_float r = _mm_mul_ps(v1, v2);
	r = _mm_hadd_ps(r, r);
	return (_mm_hadd_ps(r, r));

#else

	vec_float d = _mm_mul_ps(v1, v2);
	return (_mm_add_ss(_mm_add_ss(_mm_add_ss(d, VecSmearY(d)), VecSmearZ(d)), VecSmearW(d)));

#endif

#elif defined(NEON_ENABLED)

	vec_float d = vmulq_f32(v1, v2);
	return (vaddq_f32(vaddq_f32(vpaddq_f32(d, d), vdupq_n_f32(vgetq_lane_f32(d, 2))), vdupq_n_f32(vgetq_lane_f32(d, 3))));

#endif
}

inline vec_float VecPlaneDotPoint3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

#if defined(SSE_ENABLED4)

	const vec_float one = VecLoadScalarConstant<0x3F800000>();
	vec_float p = _mm_insert_ps(v2, one, 0x30);
	vec_float r = _mm_mul_ps(v1, p);
	r = _mm_hadd_ps(r, r);
	return (_mm_hadd_ps(r, r));

#else

	vec_float d = _mm_mul_ps(v1, v2);
	return (_mm_add_ss(_mm_add_ss(_mm_add_ss(d, VecSmearY(d)), VecSmearZ(d)), VecSmearW(v1)));

#endif

#elif defined(NEON_ENABLED)

	vec_float d = vmulq_f32(v1, v2);
	return (vaddq_f32(vaddq_f32(vpaddq_f32(d, d), vdupq_n_f32(vgetq_lane_f32(d, 2))), vdupq_n_f32(vgetq_lane_f32(v1, 3))));

#endif
}

inline vec_float VecProject3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	vec_float d = VecDot3D(v1, v2);
	return (_mm_mul_ps(v2, VecSmearX(d)));

#elif defined(NEON_ENABLED)

	vec_float d = VecDot3D(v1, v2);
	return (vmulq_f32(v2, vdupq_n_f32(vgetq_lane_f32(d, 0))));

#endif
}

inline vec_float VecReject3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	vec_float d = VecDot3D(v1, v2);
	return (_mm_sub_ps(v1, _mm_mul_ps(v2, VecSmearX(d))));

#elif defined(NEON_ENABLED)

	vec_float d = VecDot3D(v1, v2);
	return (vsubq_f32(v1, vmulq_f32(v2, vdupq_n_f32(vgetq_lane_f32(d, 0)))));

#endif
}

inline vec_float VecCross3D(const vec_float &v1, const vec_float &v2) {
#if defined(SSE_ENABLED)

	vec_float c = _mm_mul_ps(v1, _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1)));
	c = _mm_sub_ps(c, _mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1)), v2));
	return (_mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1)));

#elif defined(NEON_ENABLED)

	static const uint32_t mask[4] = { 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

	vec_uint32 m = vld1q_u32(mask);

	float32x4x2_t u1 = vuzpq_f32(v1, v1);
	float32x4x2_t u2 = vuzpq_f32(v2, v2);
	vec_float w1 = vbslq_f32(m, u1.val[0], u1.val[1]);
	vec_float w2 = vbslq_f32(m, u2.val[0], u2.val[1]);
	vec_float r = vfmsq_f32(vmulq_f32(v1, w2), w1, v2);
	float32x4x2_t s = vuzpq_f32(r, r);
	return (vbslq_f32(m, s.val[0], s.val[1]));

#endif
}

inline vec_float VecTransformVector3D(const vec_float &c1, const vec_float &c2, const vec_float &c3, const vec_float &v) {
#if defined(SSE_ENABLED)

	vec_float result = _mm_mul_ps(c1, VecSmearX(v));
	result = _mm_add_ps(result, _mm_mul_ps(c2, VecSmearY(v)));
	return (_mm_add_ps(result, _mm_mul_ps(c3, VecSmearZ(v))));

#elif defined(NEON_ENABLED)

	vec_float result = vmulq_f32(c1, VecSmearX(v));
	result = vfmaq_f32(result, c2, VecSmearY(v));
	return (vfmaq_f32(result, c3, VecSmearZ(v)));

#endif
}

inline vec_float VecTransformPoint3D(const vec_float &c1, const vec_float &c2, const vec_float &c3, const vec_float &c4, const vec_float &p) {
#if defined(SSE_ENABLED)

	vec_float result = _mm_mul_ps(c1, VecSmearX(p));
	result = _mm_add_ps(result, _mm_mul_ps(c2, VecSmearY(p)));
	result = _mm_add_ps(result, _mm_mul_ps(c3, VecSmearZ(p)));
	return (_mm_add_ps(result, c4));

#elif defined(NEON_ENABLED)

	vec_float result = vmulq_f32(c1, VecSmearX(p));
	result = vfmaq_f32(result, c2, VecSmearY(p));
	return (vaddq_f32(vfmaq_f32(result, c3, VecSmearZ(p)), c4));

#endif
}

inline vec_int8 VecInt8GetZero(void) {
#if defined(SSE_ENABLED)

	return (_mm_setzero_si128());

#elif defined(NEON_ENABLED)

	return (vmovq_n_s8(0));

#endif
}

inline vec_int16 VecInt16GetZero(void) {
#if defined(SSE_ENABLED)

	return (_mm_setzero_si128());

#elif defined(NEON_ENABLED)

	return (vmovq_n_s16(0));

#endif
}

inline vec_int32 VecInt32GetZero(void) {
#if defined(SSE_ENABLED)

	return (_mm_setzero_si128());

#elif defined(NEON_ENABLED)

	return (vmovq_n_s32(0));

#endif
}

inline vec_int8 VecInt8GetInfinity(void) {
#if defined(SSE_ENABLED)

	alignas(16) static const uint8_t int_80[16] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	return (_mm_load_si128(reinterpret_cast<const __m128i *>(int_80)));

#elif defined(NEON_ENABLED)

	return (vmovq_n_s8(-128));

#endif
}

inline vec_int8 VecInt8Load(const int8_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_load_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_s8(ptr));

#endif
}

inline vec_int8 VecInt8LoadUnaligned(const int8_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_s8(ptr));

#endif
}

inline vec_uint8 VecUInt8Load(const uint8_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_load_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_u8(ptr));

#endif
}

inline vec_uint8 VecUInt8LoadUnaligned(const uint8_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_u8(ptr));

#endif
}

inline vec_int16 VecInt16Load(const int16_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_load_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_s16(ptr));

#endif
}

inline vec_int16 VecInt16LoadUnaligned(const int16_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_s16(ptr));

#endif
}

inline vec_uint16 VecUInt16Load(const uint16_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_load_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_u16(ptr));

#endif
}

inline vec_uint16 VecUInt16LoadUnaligned(const uint16_t *ptr) {
#if defined(SSE_ENABLED)

	return (_mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr)));

#elif defined(NEON_ENABLED)

	return (vld1q_u16(ptr));

#endif
}

inline void VecInt32StoreX(const vec_int32 &v, int32_t *ptr) {
#if defined(SSE_ENABLED)

	*ptr = _mm_cvtsi128_si32(v);

#elif defined(NEON_ENABLED)

	vst1q_lane_s32(ptr, v, 0);

#endif
}

inline void VecInt32StoreY(const vec_int32 &v, int32_t *ptr) {
#if defined(SSE_ENABLED)

	*ptr = _mm_cvtsi128_si32(_mm_shuffle_epi32(v, _MM_SHUFFLE(1, 1, 1, 1)));

#elif defined(NEON_ENABLED)

	vst1q_lane_s32(ptr, v, 1);

#endif
}

inline void VecInt32StoreZ(const vec_int32 &v, int32_t *ptr) {
#if defined(SSE_ENABLED)

	*ptr = _mm_cvtsi128_si32(_mm_shuffle_epi32(v, _MM_SHUFFLE(2, 2, 2, 2)));

#elif defined(NEON_ENABLED)

	vst1q_lane_s32(ptr, v, 2);

#endif
}

inline void VecInt32StoreW(const vec_int32 &v, int32_t *ptr) {
#if defined(SSE_ENABLED)

	*ptr = _mm_cvtsi128_si32(_mm_shuffle_epi32(v, _MM_SHUFFLE(3, 3, 3, 3)));

#elif defined(NEON_ENABLED)

	vst1q_lane_s32(ptr, v, 3);

#endif
}

inline vec_int16 VecInt32PackSaturate(const vec_int32 &v) {
#if defined(SSE_ENABLED)

	return (_mm_packs_epi32(v, v));

#elif defined(NEON_ENABLED)

	int16x4_t r = vqmovn_s32(v);
	return (vcombine_s16(r, r));

#endif
}

inline vec_int16 VecInt32PackSaturate(const vec_int32 &v1, const vec_int32 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_packs_epi32(v1, v2));

#elif defined(NEON_ENABLED)

	return (vcombine_s16(vqmovn_s32(v1), vqmovn_s32(v2)));

#endif
}

inline vec_int8 VecInt16PackSaturate(const vec_int16 &v) {
#if defined(SSE_ENABLED)

	return (_mm_packs_epi16(v, v));

#elif defined(NEON_ENABLED)

	int8x8_t r = vqmovn_s16(v);
	return (vcombine_s8(r, r));

#endif
}

inline vec_uint8 VecInt16PackUnsignedSaturate(const vec_int16 &v) {
#if defined(SSE_ENABLED)

	return (_mm_packus_epi16(v, v));

#elif defined(NEON_ENABLED)

	uint8x8_t r = vqmovun_s16(v);
	return (vcombine_u8(r, r));

#endif
}

inline vec_int16 VecInt8UnpackA(const vec_int8 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpacklo_epi8(v, _mm_cmplt_epi8(v, _mm_setzero_si128())));

#elif defined(NEON_ENABLED)

	return (vmovl_s8(vget_low_s8(v)));

#endif
}

inline vec_int16 VecInt8UnpackB(const vec_int8 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpackhi_epi8(v, _mm_cmplt_epi8(v, _mm_setzero_si128())));

#elif defined(NEON_ENABLED)

	return (vmovl_s8(vget_high_s8(v)));

#endif
}

inline vec_uint16 VecUInt8UnpackA(const vec_uint8 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpacklo_epi8(v, _mm_setzero_si128()));

#elif defined(NEON_ENABLED)

	return (vmovl_u8(vget_low_u8(v)));

#endif
}

inline vec_uint16 VecUInt8UnpackB(const vec_uint8 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpackhi_epi8(v, _mm_setzero_si128()));

#elif defined(NEON_ENABLED)

	return (vmovl_u8(vget_high_u8(v)));

#endif
}

inline vec_int32 VecInt16UnpackA(const vec_int16 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpacklo_epi16(v, _mm_cmplt_epi16(v, _mm_setzero_si128())));

#elif defined(NEON_ENABLED)

	return (vmovl_s16(vget_low_s16(v)));

#endif
}

inline vec_int32 VecInt16UnpackB(const vec_int16 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpackhi_epi16(v, _mm_cmplt_epi16(v, _mm_setzero_si128())));

#elif defined(NEON_ENABLED)

	return (vmovl_s16(vget_high_s16(v)));

#endif
}

inline vec_uint32 VecUInt16UnpackA(const vec_uint16 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpacklo_epi16(v, _mm_setzero_si128()));

#elif defined(NEON_ENABLED)

	return (vmovl_u16(vget_low_u16(v)));

#endif
}

inline vec_uint32 VecUInt16UnpackB(const vec_uint16 &v) {
#if defined(SSE_ENABLED)

	return (_mm_unpackhi_epi16(v, _mm_setzero_si128()));

#elif defined(NEON_ENABLED)

	return (vmovl_u16(vget_high_u16(v)));

#endif
}

inline vec_int8 VecInt8MergeA(const vec_int8 &v1, const vec_int8 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_unpacklo_epi8(v1, v2));

#elif defined(NEON_ENABLED)

	return (vzipq_s8(v1, v2).val[0]);

#endif
}

inline vec_int8 VecInt8MergeB(const vec_int8 &v1, const vec_int8 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_unpackhi_epi8(v1, v2));

#elif defined(NEON_ENABLED)

	return (vzipq_s8(v1, v2).val[1]);

#endif
}

inline vec_uint8 VecUInt8MergeA(const vec_uint8 &v1, const vec_uint8 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_unpacklo_epi8(v1, v2));

#elif defined(NEON_ENABLED)

	return (vzipq_u8(v1, v2).val[0]);

#endif
}

inline vec_uint8 VecUInt8MergeB(const vec_uint8 &v1, const vec_uint8 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_unpackhi_epi8(v1, v2));

#elif defined(NEON_ENABLED)

	return (vzipq_u8(v1, v2).val[1]);

#endif
}

inline vec_int16 VecInt16Deinterleave(const vec_int16 &v) {
#if defined(SSE_ENABLED)

	vec_int16 i = _mm_shufflelo_epi16(v, _MM_SHUFFLE(3, 1, 2, 0));
	i = _mm_shufflehi_epi16(i, _MM_SHUFFLE(3, 1, 2, 0));
	return (_mm_shuffle_epi32(i, _MM_SHUFFLE(3, 1, 2, 0)));

#elif defined(NEON_ENABLED)

	int16x4x2_t r = vuzp_s16(vget_low_s16(v), vget_high_s16(v));
	return (vcombine_s16(r.val[0], r.val[1]));

#endif
}

inline vec_float VecInt32ConvertFloat(const vec_int32 &v) {
#if defined(SSE_ENABLED)

	return (_mm_cvtepi32_ps(v));

#elif defined(NEON_ENABLED)

	return (vcvtq_f32_s32(v));

#endif
}

inline vec_int32 VecConvertInt32(const vec_float &v) {
#if defined(SSE_ENABLED)

	return (_mm_cvtps_epi32(v));

#elif defined(NEON_ENABLED)

	return (vcvtq_s32_f32(v));

#endif
}

inline vec_int32 VecInt32Add(const vec_int32 &v1, const vec_int32 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_add_epi32(v1, v2));

#elif defined(NEON_ENABLED)

	return (vaddq_s32(v1, v2));

#endif
}

inline vec_int32 VecInt32Sub(const vec_int32 &v1, const vec_int32 &v2) {
#if defined(SSE_ENABLED)

	return (_mm_sub_epi32(v1, v2));

#elif defined(NEON_ENABLED)

	return (vsubq_s32(v1, v2));

#endif
}

#if defined(AVX_ENABLED)

inline exv_float ExvFloat(const vec_float &v1, const vec_float &v2) {
	return (_mm256_insertf128_ps(_mm256_castps128_ps256(v1), v2, 1));
}

inline exv_float ExvFloatGetZero(void) {
	return (_mm256_setzero_ps());
}

inline exv_float ExvFloatGetMinusZero(void) {
	alignas(32) static const uint32_t float_80000000[8] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
	return (_mm256_load_ps(reinterpret_cast<const float *>(float_80000000)));
}

inline exv_float ExvSmearX(const exv_float &v) {
	return (_mm256_permute_ps(v, 0x00));
}

inline exv_float ExvSmearY(const exv_float &v) {
	return (_mm256_permute_ps(v, 0x55));
}

inline exv_float ExvSmearZ(const exv_float &v) {
	return (_mm256_permute_ps(v, 0xAA));
}

inline exv_float ExvSmearW(const exv_float &v) {
	return (_mm256_permute_ps(v, 0xFF));
}

inline exv_float ExvSmearVecLo(const exv_float &v) {
	return (_mm256_permute2f128_ps(v, v, 0x00));
}

inline exv_float ExvSmearVecHi(const exv_float &v) {
	return (_mm256_permute2f128_ps(v, v, 0x11));
}

inline exv_float ExvLoad(const float *ptr) {
	return (_mm256_load_ps(ptr));
}

inline exv_float ExvLoadSmearScalar(const float *ptr) {
	return (_mm256_broadcast_ss(ptr));
}

inline void ExvStore(const exv_float &v, float *ptr) {
	_mm256_store_ps(ptr, v);
}

inline void ExvStoreX(const exv_float &v, float *ptr) {
	_mm_store_ss(ptr, _mm256_castps256_ps128(v));
}

inline void ExvStoreY(const exv_float &v, float *ptr) {
	_mm_store_ss(ptr, _mm256_castps256_ps128(_mm256_permute_ps(v, 0x55)));
}

inline void ExvStoreZ(const exv_float &v, float *ptr) {
	_mm_store_ss(ptr, _mm256_castps256_ps128(_mm256_permute_ps(v, 0xAA)));
}

inline void ExvStoreW(const exv_float &v, float *ptr) {
	_mm_store_ss(ptr, _mm256_castps256_ps128(_mm256_permute_ps(v, 0xFF)));
}

inline exv_float ExvMul(const exv_float &v1, const exv_float &v2) {
	return (_mm256_mul_ps(v1, v2));
}

inline exv_float operator*(const exv_float &v1, const exv_float &v2) {
	return (_mm256_mul_ps(v1, v2));
}

inline exv_float ExvMadd(const exv_float &v1, const exv_float &v2, const exv_float &v3) {
	return (_mm256_add_ps(_mm256_mul_ps(v1, v2), v3));
}

#endif

inline float Fabs(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecAndc(VecLoadScalar(&x), VecFloatGetMinusZero()), &result);
	return (result);

#else

	return ((x < 0.0F) ? -x : x);

#endif
}

inline float Fnabs(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecLoadScalar(&x) | VecFloatGetMinusZero(), &result);
	return (result);

#else

	return ((x > 0.0F) ? -x : x);

#endif
}

inline float Fsgn(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecFsgn(VecLoadScalar(&x)), &result);
	return (result);

#else

	return ((x < 0.0F) ? -1.0F : ((x > 0.0F) ? 1.0F : 0.0F));

#endif
}

inline float Fnsgn(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecFnsgn(VecLoadScalar(&x)), &result);
	return (result);

#else

	return ((x < 0.0F) ? 1.0F : ((x > 0.0F) ? -1.0F : 0.0F));

#endif
}

inline float NonzeroFsgn(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecNonzeroFsgn(VecLoadScalar(&x)), &result);
	return (result);

#else

	return ((x < 0.0F) ? -1.0F : 1.0F);

#endif
}

inline float NonzeroFnsgn(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecNonzeroFnsgn(VecLoadScalar(&x)), &result);
	return (result);

#else

	return ((x < 0.0F) ? 1.0F : -1.0F);

#endif
}

inline float Fmin(const float &x, const float &y) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMinScalar(VecLoadScalar(&x), VecLoadScalar(&y)), &result);
	return (result);

#else

	return ((x < y) ? x : y);

#endif
}

inline float Fmin(const float &x, const float &y, const float &z) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMinScalar(VecMinScalar(VecLoadScalar(&x), VecLoadScalar(&y)), VecLoadScalar(&z)), &result);
	return (result);

#else

	return (Fmin(Fmin(x, y), z));

#endif
}

inline float Fmin(const float &x, const float &y, const float &z, const float &w) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMinScalar(VecMinScalar(VecLoadScalar(&x), VecLoadScalar(&y)), VecMinScalar(VecLoadScalar(&z), VecLoadScalar(&w))), &result);
	return (result);

#else

	return (Fmin(Fmin(x, y), Fmin(z, w)));

#endif
}

inline float Fmax(const float &x, const float &y) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMaxScalar(VecLoadScalar(&x), VecLoadScalar(&y)), &result);
	return (result);

#else

	return ((x < y) ? y : x);

#endif
}

inline float Fmax(const float &x, const float &y, const float &z) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMaxScalar(VecMaxScalar(VecLoadScalar(&x), VecLoadScalar(&y)), VecLoadScalar(&z)), &result);
	return (result);

#else

	return (Fmax(Fmax(x, y), z));

#endif
}

inline float Fmax(const float &x, const float &y, const float &z, const float &w) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMaxScalar(VecMaxScalar(VecLoadScalar(&x), VecLoadScalar(&y)), VecMaxScalar(VecLoadScalar(&z), VecLoadScalar(&w))), &result);
	return (result);

#else

	return (Fmax(Fmax(x, y), Fmax(z, w)));

#endif
}

inline float FminZero(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMinScalar(VecLoadScalar(&x), VecFloatGetZero()), &result);
	return (result);

#else

	return ((x < 0.0F) ? x : 0.0F);

#endif
}

inline float FmaxZero(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMaxScalar(VecLoadScalar(&x), VecFloatGetZero()), &result);
	return (result);

#else

	return ((x < 0.0F) ? 0.0F : x);

#endif
}

inline float Saturate(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMinScalar(VecMaxScalar(VecLoadScalar(&x), VecFloatGetZero()), VecLoadVectorConstant<0x3F800000>()), &result);
	return (result);

#else

	float f = (x < 0.0F) ? 0.0F : x;
	return ((f < 1.0F) ? f : 1.0F);

#endif
}

inline float Clamp(const float &x, const float &y, const float &z) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecMinScalar(VecMaxScalar(VecLoadScalar(&x), VecLoadScalar(&y)), VecLoadScalar(&z)), &result);
	return (result);

#else

	float f = (x < y) ? y : x;
	return ((f < z) ? f : z);

#endif
}

#ifdef SIMD_ENABLED

inline float Floor(const float &x) {
	float result;

	VecStoreX(VecFloorScalar(VecLoadScalar(&x)), &result);
	return (result);
}

inline float Ceil(const float &x) {
	float result;

	VecStoreX(VecCeilScalar(VecLoadScalar(&x)), &result);
	return (result);
}

#else

float Floor(const float &x);
float Ceil(const float &x);

#endif

inline float PositiveFloor(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecPositiveFloorScalar(VecLoadScalar(&x)), &result);
	return (result);

#else

	return (Floor(x));

#endif
}

inline float NegativeFloor(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecNegativeFloorScalar(VecLoadScalar(&x)), &result);
	return (result);

#else

	return (Floor(x));

#endif
}

inline float PositiveCeil(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecPositiveCeilScalar(VecLoadScalar(&x)), &result);
	return (result);

#else

	return (Ceil(x));

#endif
}

inline float NegativeCeil(const float &x) {
#ifdef SIMD_ENABLED

	float result;

	VecStoreX(VecNegativeCeilScalar(VecLoadScalar(&x)), &result);
	return (result);

#else

	return (Ceil(x));

#endif
}

inline void FloorCeil(const float &x, float *f, float *c) {
#ifdef SIMD_ENABLED

	vec_float vf, vc;

	VecFloorCeilScalar(VecLoadScalar(&x), &vf, &vc);
	VecStoreX(vf, f);
	VecStoreX(vc, c);

#else

	*f = Floor(x);
	*c = Ceil(x);

#endif
}

inline void PositiveFloorCeil(const float &x, float *f, float *c) {
#ifdef SIMD_ENABLED

	vec_float vf, vc;

	VecPositiveFloorCeilScalar(VecLoadScalar(&x), &vf, &vc);
	VecStoreX(vf, f);
	VecStoreX(vc, c);

#else

	*f = Floor(x);
	*c = Ceil(x);

#endif
}

inline void NegativeFloorCeil(const float &x, float *f, float *c) {
#ifdef SIMD_ENABLED

	vec_float vf, vc;

	VecNegativeFloorCeilScalar(VecLoadScalar(&x), &vf, &vc);
	VecStoreX(vf, f);
	VecStoreX(vc, c);

#else

	*f = Floor(x);
	*c = Ceil(x);

#endif
}

inline float Frac(const float &x) {
	return (x - Floor(x));
}

inline float PositiveFrac(const float &x) {
	return (x - PositiveFloor(x));
}

inline float NegativeFrac(const float &x) {
	return (x - NegativeFloor(x));
}

#endif // MATH_DEFS_H
