/**************************************************************************/
/*  math_funcs.h                                                          */
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

#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H

#include "core/error/error_macros.h"
#include "core/math/math_defs.h"
#include "core/math/random_pcg.h"
#include "core/typedefs.h"

#include <float.h>
#include "thirdparty/misc/pcg.h"

alignas(64) const uint32_t trigTable[256][2] = {
	{ 0x3F800000, 0x00000000 }, { 0x3F7FEC43, 0x3CC90AB0 }, { 0x3F7FB10F, 0x3D48FB30 }, { 0x3F7F4E6D, 0x3D96A905 }, { 0x3F7EC46D, 0x3DC8BD36 }, { 0x3F7E1324, 0x3DFAB273 }, { 0x3F7D3AAC, 0x3E164083 }, { 0x3F7C3B28, 0x3E2F10A3 },
	{ 0x3F7B14BE, 0x3E47C5C2 }, { 0x3F79C79D, 0x3E605C13 }, { 0x3F7853F8, 0x3E78CFCD }, { 0x3F76BA07, 0x3E888E94 }, { 0x3F74FA0B, 0x3E94A031 }, { 0x3F731447, 0x3EA09AE5 }, { 0x3F710908, 0x3EAC7CD4 }, { 0x3F6ED89E, 0x3EB8442A },
	{ 0x3F6C835E, 0x3EC3EF16 }, { 0x3F6A09A6, 0x3ECF7BCB }, { 0x3F676BD8, 0x3EDAE880 }, { 0x3F64AA59, 0x3EE63375 }, { 0x3F61C597, 0x3EF15AEA }, { 0x3F5EBE05, 0x3EFC5D28 }, { 0x3F5B941A, 0x3F039C3D }, { 0x3F584853, 0x3F08F59B },
	{ 0x3F54DB31, 0x3F0E39DA }, { 0x3F514D3D, 0x3F13682B }, { 0x3F4D9F02, 0x3F187FC0 }, { 0x3F49D112, 0x3F1D7FD2 }, { 0x3F45E403, 0x3F22679A }, { 0x3F41D870, 0x3F273656 }, { 0x3F3DAEF9, 0x3F2BEB4A }, { 0x3F396842, 0x3F3085BB },
	{ 0x3F3504F3, 0x3F3504F3 }, { 0x3F3085BA, 0x3F396842 }, { 0x3F2BEB49, 0x3F3DAEFA }, { 0x3F273655, 0x3F41D871 }, { 0x3F226799, 0x3F45E403 }, { 0x3F1D7FD1, 0x3F49D112 }, { 0x3F187FC0, 0x3F4D9F02 }, { 0x3F13682A, 0x3F514D3D },
	{ 0x3F0E39D9, 0x3F54DB32 }, { 0x3F08F59B, 0x3F584853 }, { 0x3F039C3C, 0x3F5B941B }, { 0x3EFC5D27, 0x3F5EBE05 }, { 0x3EF15AE7, 0x3F61C598 }, { 0x3EE63374, 0x3F64AA59 }, { 0x3EDAE881, 0x3F676BD8 }, { 0x3ECF7BC9, 0x3F6A09A7 },
	{ 0x3EC3EF15, 0x3F6C835E }, { 0x3EB84427, 0x3F6ED89E }, { 0x3EAC7CD3, 0x3F710908 }, { 0x3EA09AE2, 0x3F731448 }, { 0x3E94A030, 0x3F74FA0B }, { 0x3E888E93, 0x3F76BA07 }, { 0x3E78CFC8, 0x3F7853F8 }, { 0x3E605C12, 0x3F79C79D },
	{ 0x3E47C5BC, 0x3F7B14BF }, { 0x3E2F10A0, 0x3F7C3B28 }, { 0x3E164085, 0x3F7D3AAC }, { 0x3DFAB26C, 0x3F7E1324 }, { 0x3DC8BD35, 0x3F7EC46D }, { 0x3D96A8FB, 0x3F7F4E6D }, { 0x3D48FB29, 0x3F7FB10F }, { 0x3CC90A7E, 0x3F7FEC43 },
	{ 0x00000000, 0x3F800000 }, { 0xBCC90A7E, 0x3F7FEC43 }, { 0xBD48FB29, 0x3F7FB10F }, { 0xBD96A8FB, 0x3F7F4E6D }, { 0xBDC8BD35, 0x3F7EC46D }, { 0xBDFAB26C, 0x3F7E1324 }, { 0xBE164085, 0x3F7D3AAC }, { 0xBE2F10A0, 0x3F7C3B28 },
	{ 0xBE47C5BC, 0x3F7B14BF }, { 0xBE605C12, 0x3F79C79D }, { 0xBE78CFC8, 0x3F7853F8 }, { 0xBE888E93, 0x3F76BA07 }, { 0xBE94A030, 0x3F74FA0B }, { 0xBEA09AE2, 0x3F731448 }, { 0xBEAC7CD3, 0x3F710908 }, { 0xBEB84427, 0x3F6ED89E },
	{ 0xBEC3EF15, 0x3F6C835E }, { 0xBECF7BC9, 0x3F6A09A7 }, { 0xBEDAE881, 0x3F676BD8 }, { 0xBEE63374, 0x3F64AA59 }, { 0xBEF15AE7, 0x3F61C598 }, { 0xBEFC5D27, 0x3F5EBE05 }, { 0xBF039C3C, 0x3F5B941B }, { 0xBF08F59B, 0x3F584853 },
	{ 0xBF0E39D9, 0x3F54DB32 }, { 0xBF13682A, 0x3F514D3D }, { 0xBF187FC0, 0x3F4D9F02 }, { 0xBF1D7FD1, 0x3F49D112 }, { 0xBF226799, 0x3F45E403 }, { 0xBF273655, 0x3F41D871 }, { 0xBF2BEB49, 0x3F3DAEFA }, { 0xBF3085BA, 0x3F396842 },
	{ 0xBF3504F3, 0x3F3504F3 }, { 0xBF396842, 0x3F3085BB }, { 0xBF3DAEF9, 0x3F2BEB4A }, { 0xBF41D870, 0x3F273656 }, { 0xBF45E403, 0x3F22679A }, { 0xBF49D112, 0x3F1D7FD2 }, { 0xBF4D9F02, 0x3F187FC0 }, { 0xBF514D3D, 0x3F13682B },
	{ 0xBF54DB31, 0x3F0E39DA }, { 0xBF584853, 0x3F08F59B }, { 0xBF5B941A, 0x3F039C3D }, { 0xBF5EBE05, 0x3EFC5D28 }, { 0xBF61C597, 0x3EF15AEA }, { 0xBF64AA59, 0x3EE63375 }, { 0xBF676BD8, 0x3EDAE880 }, { 0xBF6A09A6, 0x3ECF7BCB },
	{ 0xBF6C835E, 0x3EC3EF16 }, { 0xBF6ED89E, 0x3EB8442A }, { 0xBF710908, 0x3EAC7CD4 }, { 0xBF731447, 0x3EA09AE5 }, { 0xBF74FA0B, 0x3E94A031 }, { 0xBF76BA07, 0x3E888E94 }, { 0xBF7853F8, 0x3E78CFCD }, { 0xBF79C79D, 0x3E605C13 },
	{ 0xBF7B14BE, 0x3E47C5C2 }, { 0xBF7C3B28, 0x3E2F10A3 }, { 0xBF7D3AAC, 0x3E164083 }, { 0xBF7E1324, 0x3DFAB273 }, { 0xBF7EC46D, 0x3DC8BD36 }, { 0xBF7F4E6D, 0x3D96A905 }, { 0xBF7FB10F, 0x3D48FB30 }, { 0xBF7FEC43, 0x3CC90AB0 },
	{ 0xBF800000, 0x00000000 }, { 0xBF7FEC43, 0xBCC90AB0 }, { 0xBF7FB10F, 0xBD48FB30 }, { 0xBF7F4E6D, 0xBD96A905 }, { 0xBF7EC46D, 0xBDC8BD36 }, { 0xBF7E1324, 0xBDFAB273 }, { 0xBF7D3AAC, 0xBE164083 }, { 0xBF7C3B28, 0xBE2F10A3 },
	{ 0xBF7B14BE, 0xBE47C5C2 }, { 0xBF79C79D, 0xBE605C13 }, { 0xBF7853F8, 0xBE78CFCD }, { 0xBF76BA07, 0xBE888E94 }, { 0xBF74FA0B, 0xBE94A031 }, { 0xBF731447, 0xBEA09AE5 }, { 0xBF710908, 0xBEAC7CD4 }, { 0xBF6ED89E, 0xBEB8442A },
	{ 0xBF6C835E, 0xBEC3EF16 }, { 0xBF6A09A6, 0xBECF7BCB }, { 0xBF676BD8, 0xBEDAE880 }, { 0xBF64AA59, 0xBEE63375 }, { 0xBF61C597, 0xBEF15AEA }, { 0xBF5EBE05, 0xBEFC5D28 }, { 0xBF5B941A, 0xBF039C3D }, { 0xBF584853, 0xBF08F59B },
	{ 0xBF54DB31, 0xBF0E39DA }, { 0xBF514D3D, 0xBF13682B }, { 0xBF4D9F02, 0xBF187FC0 }, { 0xBF49D112, 0xBF1D7FD2 }, { 0xBF45E403, 0xBF22679A }, { 0xBF41D870, 0xBF273656 }, { 0xBF3DAEF9, 0xBF2BEB4A }, { 0xBF396842, 0xBF3085BB },
	{ 0xBF3504F3, 0xBF3504F3 }, { 0xBF3085BA, 0xBF396842 }, { 0xBF2BEB49, 0xBF3DAEFA }, { 0xBF273655, 0xBF41D871 }, { 0xBF226799, 0xBF45E403 }, { 0xBF1D7FD1, 0xBF49D112 }, { 0xBF187FC0, 0xBF4D9F02 }, { 0xBF13682A, 0xBF514D3D },
	{ 0xBF0E39D9, 0xBF54DB32 }, { 0xBF08F59B, 0xBF584853 }, { 0xBF039C3C, 0xBF5B941B }, { 0xBEFC5D27, 0xBF5EBE05 }, { 0xBEF15AE7, 0xBF61C598 }, { 0xBEE63374, 0xBF64AA59 }, { 0xBEDAE881, 0xBF676BD8 }, { 0xBECF7BC9, 0xBF6A09A7 },
	{ 0xBEC3EF15, 0xBF6C835E }, { 0xBEB84427, 0xBF6ED89E }, { 0xBEAC7CD3, 0xBF710908 }, { 0xBEA09AE2, 0xBF731448 }, { 0xBE94A030, 0xBF74FA0B }, { 0xBE888E93, 0xBF76BA07 }, { 0xBE78CFC8, 0xBF7853F8 }, { 0xBE605C12, 0xBF79C79D },
	{ 0xBE47C5BC, 0xBF7B14BF }, { 0xBE2F10A0, 0xBF7C3B28 }, { 0xBE164085, 0xBF7D3AAC }, { 0xBDFAB26C, 0xBF7E1324 }, { 0xBDC8BD35, 0xBF7EC46D }, { 0xBD96A8FB, 0xBF7F4E6D }, { 0xBD48FB29, 0xBF7FB10F }, { 0xBCC90A7E, 0xBF7FEC43 },
	{ 0x00000000, 0xBF800000 }, { 0x3CC90A7E, 0xBF7FEC43 }, { 0x3D48FB29, 0xBF7FB10F }, { 0x3D96A8FB, 0xBF7F4E6D }, { 0x3DC8BD35, 0xBF7EC46D }, { 0x3DFAB26C, 0xBF7E1324 }, { 0x3E164085, 0xBF7D3AAC }, { 0x3E2F10A0, 0xBF7C3B28 },
	{ 0x3E47C5BC, 0xBF7B14BF }, { 0x3E605C12, 0xBF79C79D }, { 0x3E78CFC8, 0xBF7853F8 }, { 0x3E888E93, 0xBF76BA07 }, { 0x3E94A030, 0xBF74FA0B }, { 0x3EA09AE2, 0xBF731448 }, { 0x3EAC7CD3, 0xBF710908 }, { 0x3EB84427, 0xBF6ED89E },
	{ 0x3EC3EF15, 0xBF6C835E }, { 0x3ECF7BC9, 0xBF6A09A7 }, { 0x3EDAE881, 0xBF676BD8 }, { 0x3EE63374, 0xBF64AA59 }, { 0x3EF15AE7, 0xBF61C598 }, { 0x3EFC5D27, 0xBF5EBE05 }, { 0x3F039C3C, 0xBF5B941B }, { 0x3F08F59B, 0xBF584853 },
	{ 0x3F0E39D9, 0xBF54DB32 }, { 0x3F13682A, 0xBF514D3D }, { 0x3F187FC0, 0xBF4D9F02 }, { 0x3F1D7FD1, 0xBF49D112 }, { 0x3F226799, 0xBF45E403 }, { 0x3F273655, 0xBF41D871 }, { 0x3F2BEB49, 0xBF3DAEFA }, { 0x3F3085BA, 0xBF396842 },
	{ 0x3F3504F3, 0xBF3504F3 }, { 0x3F396842, 0xBF3085BB }, { 0x3F3DAEF9, 0xBF2BEB4A }, { 0x3F41D870, 0xBF273656 }, { 0x3F45E403, 0xBF22679A }, { 0x3F49D112, 0xBF1D7FD2 }, { 0x3F4D9F02, 0xBF187FC0 }, { 0x3F514D3D, 0xBF13682B },
	{ 0x3F54DB31, 0xBF0E39DA }, { 0x3F584853, 0xBF08F59B }, { 0x3F5B941A, 0xBF039C3D }, { 0x3F5EBE05, 0xBEFC5D28 }, { 0x3F61C597, 0xBEF15AEA }, { 0x3F64AA59, 0xBEE63375 }, { 0x3F676BD8, 0xBEDAE880 }, { 0x3F6A09A6, 0xBECF7BCB },
	{ 0x3F6C835E, 0xBEC3EF16 }, { 0x3F6ED89E, 0xBEB8442A }, { 0x3F710908, 0xBEAC7CD4 }, { 0x3F731447, 0xBEA09AE5 }, { 0x3F74FA0B, 0xBE94A031 }, { 0x3F76BA07, 0xBE888E94 }, { 0x3F7853F8, 0xBE78CFCD }, { 0x3F79C79D, 0xBE605C13 },
	{ 0x3F7B14BE, 0xBE47C5C2 }, { 0x3F7C3B28, 0xBE2F10A3 }, { 0x3F7D3AAC, 0xBE164083 }, { 0x3F7E1324, 0xBDFAB273 }, { 0x3F7EC46D, 0xBDC8BD36 }, { 0x3F7F4E6D, 0xBD96A905 }, { 0x3F7FB10F, 0xBD48FB30 }, { 0x3F7FEC43, 0xBCC90AB0 }
};

struct TrigPair {
	float x;
	float y;
};

inline const TrigPair *GetTrigTable(void) {
	return (reinterpret_cast<const TrigPair *>(trigTable));
}


class Math {
	static RandomPCG default_rand;

public:
	Math() {} // useless to instance

	// Not using 'RANDOM_MAX' to avoid conflict with system headers on some OSes (at least NetBSD).
	static const uint64_t RANDOM_32BIT_MAX = 0xFFFFFFFF;

	static float Sqrt(float x) {
#ifdef SSE_ENABLED

		float result;

		vec_float v = _mm_load_ss(&x);
		vec_float mask = _mm_cmplt_ss(v, VecLoadScalarConstant<0x00800000>());

		vec_float r = _mm_rsqrt_ss(v);
		r = _mm_mul_ss(_mm_sub_ss(VecLoadScalarConstant<0x40400000>(), _mm_mul_ss(v, _mm_mul_ss(r, r))), _mm_mul_ss(r, VecLoadScalarConstant<0x3F000001>()));

		_mm_store_ss(&result, _mm_mul_ss(_mm_andnot_ps(mask, r), v));
		return (result);

#else

		if (x < Math_min_float) {
			return (0.0F);
		}

		uint32_t i = 0x5F375A86 - (asuint(x) >> 1);
		float r = asfloat(i);
		r = (0.5F * r) * (3.0F - x * r * r);
		r = (0.5F * r) * (3.0F - x * r * r);
		return (r * x);

#endif
	}

	static float InverseSqrt(float x) {
#ifdef SSE_ENABLED

		float result;

		vec_float v = _mm_load_ss(&x);
		vec_float mask = _mm_cmplt_ss(v, VecLoadScalarConstant<0x00800000>());

		vec_float r = _mm_rsqrt_ss(v);
		r = _mm_mul_ss(_mm_sub_ss(VecLoadScalarConstant<0x40400000>(), _mm_mul_ss(v, _mm_mul_ss(r, r))), _mm_mul_ss(r, VecLoadScalarConstant<0x3F000001>()));

		_mm_store_ss(&result, _mm_or_ps(_mm_andnot_ps(mask, r), _mm_and_ps(mask, VecLoadScalarConstant<0x7F800000>())));
		return (result);

#else

		if (x < Math_min_float) {
			return (Math_infinity);
		}

		uint32_t i = 0x5F375A86 - (asuint(x) >> 1);
		float r = asfloat(i);
		r = (0.5F * r) * (3.0F - x * r * r);
		r = (0.5F * r) * (3.0F - x * r * r);
		return (r);

#endif
	}

	static float sin(float x) {
	#ifdef SIMD_ENABLED

		float result;

		vec_float b = VecMulScalar(VecAndc(VecLoadScalar(&x), VecFloatGetMinusZero()), VecLoadScalarConstant<0x4222F983>());
		vec_float i = VecPositiveFloorScalar(b);
		b = VecMulScalar(VecSubScalar(b, i), VecLoadScalarConstant<0x3CC90FDB>());

		const TrigPair &cossin = GetTrigTable()[VecTruncateConvert(i) & 255];
		vec_float cosine_alpha = VecLoadScalar(&cossin.x);
		vec_float sine_alpha = VecLoadScalar(&cossin.y);

		vec_float b2 = VecMulScalar(b, b);
		vec_float sine_beta = VecNmsubScalar(VecMulScalar(b, b2), VecNmsubScalar(b2, VecLoadScalarConstant<0x3E2AAAAB>(), VecLoadScalarConstant<0x3C088889>()), b);
		vec_float cosine_beta = VecNmsubScalar(b2, VecNmsub(b2, VecLoadScalarConstant<0x3D2AAAAB>(), VecLoadScalarConstant<0x3F000000>()), VecLoadScalarConstant<0x3F800000>());

		vec_float sine = VecMaddScalar(sine_alpha, cosine_beta, VecMulScalar(cosine_alpha, sine_beta));

		VecStoreX((x < 0.0F) ? -sine : sine, &result);
		return (result);

	#else

		float b = Fabs(x) * 40.74366543F; // 256 / tau
		float i = PositiveFloor(b);
		b = (b - i) * 0.0245436926F; // tau / 256

		const TrigPair &cossin_alpha = GetTrigTable()[int32_t(i) & 255];

		float b2 = b * b;
		float sine_beta = b - b * b2 * (0.1666666667F - b2 * 0.008333333333F);
		float cosine_beta = 1.0F - b2 * (0.5F - b2 * 0.04166666667F);

		float sine = cossin_alpha.y * cosine_beta + cossin_alpha.x * sine_beta;
		return ((x < 0.0F) ? -sine : sine);

	#endif
	}

	static float cos(float x) {
	#ifdef SIMD_ENABLED

		float result;

		vec_float b = VecMulScalar(VecAndc(VecLoadScalar(&x), VecFloatGetMinusZero()), VecLoadScalarConstant<0x4222F983>());
		vec_float i = VecPositiveFloorScalar(b);
		b = VecMulScalar(VecSubScalar(b, i), VecLoadScalarConstant<0x3CC90FDB>());

		const TrigPair &cossin = GetTrigTable()[VecTruncateConvert(i) & 255];
		vec_float cosine_alpha = VecLoadScalar(&cossin.x);
		vec_float sine_alpha = VecLoadScalar(&cossin.y);

		vec_float b2 = VecMulScalar(b, b);
		vec_float sine_beta = VecNmsubScalar(VecMulScalar(b, b2), VecNmsubScalar(b2, VecLoadScalarConstant<0x3E2AAAAB>(), VecLoadScalarConstant<0x3C088889>()), b);
		vec_float cosine_beta = VecNmsubScalar(b2, VecNmsub(b2, VecLoadScalarConstant<0x3D2AAAAB>(), VecLoadScalarConstant<0x3F000000>()), VecLoadScalarConstant<0x3F800000>());

		VecStoreX(VecSubScalar(VecMulScalar(cosine_alpha, cosine_beta), VecMulScalar(sine_alpha, sine_beta)), &result);
		return (result);

	#else

		float b = Fabs(x) * 40.74366543F; // 256 / tau
		float i = PositiveFloor(b);
		b = (b - i) * 0.0245436926F; // tau / 256

		const TrigPair &cossin_alpha = GetTrigTable()[int32_t(i) & 255];

		float b2 = b * b;
		float sine_beta = b - b * b2 * (0.1666666667F - b2 * 0.008333333333F);
		float cosine_beta = 1.0F - b2 * (0.5F - b2 * 0.04166666667F);

		return (cossin_alpha.x * cosine_beta - cossin_alpha.y * sine_beta);

	#endif
	}

	static float tan(float x) {
	#ifdef SIMD_ENABLED

		float cosine, sine;

		vec_float b = VecMulScalar(VecAndc(VecLoadScalar(&x), VecFloatGetMinusZero()), VecLoadScalarConstant<0x4222F983>());
		vec_float i = VecPositiveFloorScalar(b);
		b = VecMulScalar(VecSubScalar(b, i), VecLoadScalarConstant<0x3CC90FDB>());

		const TrigPair &cossin = GetTrigTable()[VecTruncateConvert(i) & 255];
		vec_float cosine_alpha = VecLoadScalar(&cossin.x);
		vec_float sine_alpha = VecLoadScalar(&cossin.y);

		vec_float b2 = VecMulScalar(b, b);
		vec_float sine_beta = VecNmsubScalar(VecMulScalar(b, b2), VecNmsubScalar(b2, VecLoadScalarConstant<0x3E2AAAAB>(), VecLoadScalarConstant<0x3C088889>()), b);
		vec_float cosine_beta = VecNmsubScalar(b2, VecNmsub(b2, VecLoadScalarConstant<0x3D2AAAAB>(), VecLoadScalarConstant<0x3F000000>()), VecLoadScalarConstant<0x3F800000>());

		VecStoreX(VecMaddScalar(sine_alpha, cosine_beta, VecMulScalar(cosine_alpha, sine_beta)), &sine);
		VecStoreX(VecSubScalar(VecMulScalar(cosine_alpha, cosine_beta), VecMulScalar(sine_alpha, sine_beta)), &cosine);

		float result = sine / cosine;
		return ((x < 0.0F) ? -result : result);

	#else

		float b = Fabs(x) * 40.74366543F; // 256 / tau
		float i = PositiveFloor(b);
		b = (b - i) * 0.0245436926F; // tau / 256

		const TrigPair &cossin_alpha = GetTrigTable()[int32_t(i) & 255];

		float b2 = b * b;
		float sine_beta = b - b * b2 * (0.1666666667F - b2 * 0.008333333333F);
		float cosine_beta = 1.0F - b2 * (0.5F - b2 * 0.04166666667F);

		float sine = cossin_alpha.y * cosine_beta + cossin_alpha.x * sine_beta;
		float cosine = cossin_alpha.x * cosine_beta - cossin_alpha.y * sine_beta;

		float result = sine / cosine;
		return ((x < 0.0F) ? -result : result);

	#endif
	}

	static float exp(float x) {
		// Values of exp(n) for integers n in the range [-88, 91].

		alignas(64) static const uint32_t table[180] = {
			0x00000000, 0x00B33687, 0x01739362, 0x022586E0, 0x02E0F96D, 0x0398E2CB, 0x044FCB22, 0x050D35D7,
			0x05BFECBA, 0x06826D27, 0x07314490, 0x07F0EE94, 0x08A3BAF0, 0x095E884F, 0x0A1739FB, 0x0ACD89C1,
			0x0B8BAD78, 0x0C3DD771, 0x0D0102BF, 0x0DAF5800, 0x0E6E511E, 0x0F21F3FE, 0x0FDC1DF9, 0x109595C7,
			0x114B4EA4, 0x120A295C, 0x12BBC7F1, 0x137F388B, 0x142D70C9, 0x14EBBAEC, 0x15A031FC, 0x1659BA5A,
			0x1713F623, 0x17C919B9, 0x1888A975, 0x1939BE2B, 0x19FC7361, 0x1AAB8EDC, 0x1B692BEB, 0x1C1E74DD,
			0x1CD75D5D, 0x1D925B02, 0x1E46EAF1, 0x1F072DBA, 0x1FB7BA0F, 0x2079B5EA, 0x2129B229, 0x21E6A405,
			0x229CBC92, 0x235506F2, 0x2410C457, 0x24C4C239, 0x2585B61D, 0x2635BB8D, 0x26F7000F, 0x27A7DAA4,
			0x28642328, 0x291B090F, 0x29D2B706, 0x2A8F3216, 0x2B429F81, 0x2C044295, 0x2CB3C295, 0x2D7451BD,
			0x2E26083C, 0x2EE1A93F, 0x2F995A46, 0x30506D87, 0x310DA433, 0x31C082B8, 0x3282D314, 0x3331CF19,
			0x33F1AADE, 0x34A43AE5, 0x355F3638, 0x3617B02A, 0x36CE2A62, 0x378C1AA1, 0x383E6BCE, 0x39016791,
			0x39AFE108, 0x3A6F0B5D, 0x3B227290, 0x3BDCC9FF, 0x3C960AAE, 0x3D4BED86, 0x3E0A9555, 0x3EBC5AB2,
			0x3F800000, 0x402DF854, 0x40EC7326, 0x41A0AF2E, 0x425A6481, 0x431469C5, 0x43C9B6E3, 0x44891443,
			0x453A4F54, 0x45FD38AC, 0x46AC14EE, 0x4769E224, 0x481EF0B3, 0x48D805AD, 0x4992CD62, 0x4A478665,
			0x4B07975F, 0x4BB849A4, 0x4C7A7910, 0x4D2A36C8, 0x4DE75844, 0x4E9D3710, 0x4F55AD6E, 0x5011357A,
			0x50C55BFE, 0x51861E9D, 0x52364993, 0x52F7C118, 0x53A85DD2, 0x5464D572, 0x551B8238, 0x55D35BB3,
			0x568FA1FE, 0x5743379A, 0x5804A9F1, 0x58B44F11, 0x597510AD, 0x5A2689FE, 0x5AE2599A, 0x5B99D21F,
			0x5C51106A, 0x5D0E12E4, 0x5DC1192B, 0x5E833952, 0x5F325A0E, 0x5FF267BB, 0x60A4BB3E, 0x615FE4A9,
			0x621826B5, 0x62CECB81, 0x638C881F, 0x643F009E, 0x6501CCB3, 0x65B06A7B, 0x666FC62D, 0x6722F184,
			0x67DD768B, 0x68967FF0, 0x694C8CE5, 0x6A0B01A3, 0x6ABCEDE5, 0x6B806408, 0x6C2E804A, 0x6CED2BEF,
			0x6DA12CC1, 0x6E5B0F2E, 0x6F14DDC1, 0x6FCA5487, 0x70897F64, 0x713AE0EE, 0x71FDFE91, 0x72AC9B6A,
			0x736A98EC, 0x741F6CE9, 0x74D8AE7F, 0x7593401C, 0x76482254, 0x77080156, 0x77B8D9AA, 0x787B3CCF,
			0x792ABBCE, 0x79E80D11, 0x7A9DB1ED, 0x7B56546B, 0x7C11A6F5, 0x7CC5F63B, 0x7D86876D, 0x7E36D809,
			0x7EF882B7, 0x7F800000
		};

		float f = Floor(x);
		int32_t i = int32_t(f) + 88;
		if (i > 0) {
			x -= f;

			float r = 1.0F / 362880.0F;
			r = r * x + 1.0F / 40320.0F;
			r = r * x + 1.0F / 5040.0F;
			r = r * x + 1.0F / 720.0F;
			r = r * x + 1.0F / 120.0F;
			r = r * x + 1.0F / 24.0F;
			r = r * x + 1.0F / 6.0F;
			r = r * x + 0.5F;

			return ((r * (x * x) + (x + 1.0F)) * reinterpret_cast<const float *>(table)[(i < 179) ? i : 179]);
		}

		return (0.0F);
	}

	static float log(float x) {
		int32_t i = reinterpret_cast<int32_t &>(x);
		if (i > 0) {
			int32_t e = (i >> 23) - 127;
			i = (i & 0x007FFFFF) | 0x3F800000;
			float f = asfloat(i);
			float y = (f - 1.0F) / (f + 1.0F);
			float z = y * y;

			float r = 1.0F / 13.0F;
			r = r * z + 1.0F / 11.0F;
			r = r * z + 1.0F / 9.0F;
			r = r * z + 1.0F / 7.0F;
			r = r * z + 1.0F / 5.0F;
			r = r * z + 1.0F / 3.0F;
			r = r * z + 1.0F;

			return (r * (y * 2.0F) + float(e) * Math_ln_2);
		} else if (x >= 0.0F) {
			return (Math_minus_infinity);
		}

		static const uint32_t nan = 0x7FFFFFFF;
		return (asfloat(nan));
	}

	static _ALWAYS_INLINE_ float pow(float base, float exponent) {
		return (exp(log(base) * exponent));
	}

	static float arctan(float x) {
		// Values of arctan(n / 64) for integers n in the range [0, 64].

		alignas(64) static const uint32_t table[65] = {
			0x00000000, 0x3C7FFAAB, 0x3CFFEAAE, 0x3D3FDC0C, 0x3D7FAADE, 0x3D9FACF8, 0x3DBF70C1, 0x3DDF1CF6,
			0x3DFEADD5, 0x3E0F0FD8, 0x3E1EB777, 0x3E2E4C09, 0x3E3DCBDA, 0x3E4D3547, 0x3E5C86BB, 0x3E6BBEAF,
			0x3E7ADBB0, 0x3E84EE2D, 0x3E8C5FAD, 0x3E93C1B9, 0x3E9B13BA, 0x3EA25522, 0x3EA9856D, 0x3EB0A420,
			0x3EB7B0CA, 0x3EBEAB02, 0x3EC5926A, 0x3ECC66AA, 0x3ED32776, 0x3ED9D489, 0x3EE06DA6, 0x3EE6F29A,
			0x3EED6338, 0x3EF3BF5C, 0x3EFA06E8, 0x3F001CE4, 0x3F032BF5, 0x3F0630A3, 0x3F092AED, 0x3F0C1AD4,
			0x3F0F005D, 0x3F11DB8F, 0x3F14AC73, 0x3F177314, 0x3F1A2F81, 0x3F1CE1C9, 0x3F1F89FE, 0x3F222833,
			0x3F24BC7D, 0x3F2746F3, 0x3F29C7AC, 0x3F2C3EC1, 0x3F2EAC4C, 0x3F311069, 0x3F336B32, 0x3F35BCC5,
			0x3F38053E, 0x3F3A44BC, 0x3F3C7B5E, 0x3F3EA941, 0x3F40CE86, 0x3F42EB4B, 0x3F44FFB0, 0x3F470BD5, 0x3F490FDB
		};

		// arctan(a) = arctan(b) + arctan((a - b) / (ab + 1))

		float a = Fabs(x);
		if (a <= 1.0F) {
			float b = a * 64.0F;
			float i = PositiveFloor(b);
			b = i * 0.015625F;

			float arctan_b = reinterpret_cast<const float *>(table)[int32_t(i)];
			float c = (a - b) / (a * b + 1.0F);
			float c2 = c * c;

			float arctan_c = c * (1.0F - c2 * (0.3333333333F + c2 * (0.2F - c2 * 0.1428571429F)));
			a = arctan_b + arctan_c;
		} else {
			// arctan(a) = tau / 4 - arctan(1 / a)

			a = 1.0F / a;
			float b = a * 64.0F;
			float i = PositiveFloor(b);
			b = i * 0.015625F;

			float arctan_b = reinterpret_cast<const float *>(table)[int32_t(i)];
			float c = (a - b) / (a * b + 1.0F);
			float c2 = c * c;

			float arctan_c = c * (1.0F - c2 * (0.3333333333F + c2 * (0.2F - c2 * 0.1428571429F)));
			a = Math_tau_over_4 - (arctan_b + arctan_c);
		}

		return ((x < 0.0F) ? -a : a);
	}

	static float arctan(float y, float x) {
		if (Fabs(y) > Math_min_float) {
			if (Fabs(x) > Math_min_float) {
				float r = arctan(y / x);

				if (x < 0.0F) {
					if (y >= 0.0F) {
						r += Math_tau_over_2;
					} else {
						r -= Math_tau_over_2;
					}
				}

				return (r);
			}

			return ((y < 0.0F) ? -Math_tau_over_4 : Math_tau_over_4);
		}

		return ((x < 0.0F) ? Math_tau_over_2 : 0.0F);
	}

	static float arcsin(float y) {
		return (arctan(y * InverseSqrt(1.0F - y * y)));
	}

	static float arccos(float x) {
		float r = arctan(Sqrt(1.0F - x * x) / x);
		return ((x < 0.0F) ? r + Math_tau_over_2 : r);
	}

	static _ALWAYS_INLINE_ double sinh(double p_x) { return ::sinh(p_x); }
	static _ALWAYS_INLINE_ float sinh(float p_x) { return ::sinhf(p_x); }

	static _ALWAYS_INLINE_ float sinc(float p_x) { return p_x == 0 ? 1 : sin(p_x) / p_x; }

	static _ALWAYS_INLINE_ float sincn(float p_x) { return sinc(Math_tau_over_2 * p_x); }

	static _ALWAYS_INLINE_ double cosh(double p_x) { return ::cosh(p_x); }
	static _ALWAYS_INLINE_ float cosh(float p_x) { return ::coshf(p_x); }

	static _ALWAYS_INLINE_ double tanh(double p_x) { return ::tanh(p_x); }
	static _ALWAYS_INLINE_ float tanh(float p_x) { return ::tanhf(p_x); }

	// Always does clamping so always safe to use.
	static _ALWAYS_INLINE_ float asin(float p_x) { return p_x < -1 ? (-Math_tau_over_4) : (p_x > 1 ? (Math_tau_over_4) : arcsin(p_x)); }

	// Always does clamping so always safe to use.
	static _ALWAYS_INLINE_ float acos(float p_x) { return p_x < -1 ? Math_tau_over_2 : (p_x > 1 ? 0 : arccos(p_x)); }

	static _ALWAYS_INLINE_ float atan(float p_x) { return arctan(p_x); }

	static _ALWAYS_INLINE_ float atan2(float p_y, float p_x) { return arctan(p_y, p_x); }

	static _ALWAYS_INLINE_ double asinh(double p_x) { return ::asinh(p_x); }
	static _ALWAYS_INLINE_ float asinh(float p_x) { return ::asinhf(p_x); }

	// Always does clamping so always safe to use.
	static _ALWAYS_INLINE_ double acosh(double p_x) { return p_x < 1 ? 0 : ::acosh(p_x); }
	static _ALWAYS_INLINE_ float acosh(float p_x) { return p_x < 1 ? 0 : ::acoshf(p_x); }

	// Always does clamping so always safe to use.
	static _ALWAYS_INLINE_ double atanh(double p_x) { return p_x <= -1 ? -INFINITY : (p_x >= 1 ? INFINITY : ::atanh(p_x)); }
	static _ALWAYS_INLINE_ float atanh(float p_x) { return p_x <= -1 ? -INFINITY : (p_x >= 1 ? INFINITY : ::atanhf(p_x)); }

	static _ALWAYS_INLINE_ float sqrt(float p_x) { return Sqrt(p_x); }

	static _ALWAYS_INLINE_ double fmod(double p_x, double p_y) { return ::fmod(p_x, p_y); }
	static _ALWAYS_INLINE_ float fmod(float p_x, float p_y) { return ::fmodf(p_x, p_y); }

	static _ALWAYS_INLINE_ double modf(double p_x, double *r_y) { return ::modf(p_x, r_y); }
	static _ALWAYS_INLINE_ float modf(float p_x, float *r_y) { return ::modff(p_x, r_y); }

	static _ALWAYS_INLINE_ float floor(float p_x) { return Floor(p_x); }
	static _ALWAYS_INLINE_ float ceil(float p_x) { return Ceil(p_x); }

	static _ALWAYS_INLINE_ double log1p(double p_x) { return ::log1p(p_x); }
	static _ALWAYS_INLINE_ float log1p(float p_x) { return ::log1pf(p_x); }

	static _ALWAYS_INLINE_ double log2(double p_x) { return ::log2(p_x); }
	static _ALWAYS_INLINE_ float log2(float p_x) { return ::log2f(p_x); }

	static _ALWAYS_INLINE_ bool is_nan(double p_val) {
#ifdef _MSC_VER
		return _isnan(p_val);
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint64_t u;
			double f;
		} ieee754;
		ieee754.f = p_val;
		// (unsigned)(0x7ff0000000000001 >> 32) : 0x7ff00000
		return ((((unsigned)(ieee754.u >> 32) & 0x7fffffff) + ((unsigned)ieee754.u != 0)) > 0x7ff00000);
#else
		return isnan(p_val);
#endif
	}

	static _ALWAYS_INLINE_ bool is_nan(float p_val) {
#ifdef _MSC_VER
		return _isnan(p_val);
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint32_t u;
			float f;
		} ieee754;
		ieee754.f = p_val;
		// -----------------------------------
		// (single-precision floating-point)
		// NaN : s111 1111 1xxx xxxx xxxx xxxx xxxx xxxx
		//     : (> 0x7f800000)
		// where,
		//   s : sign
		//   x : non-zero number
		// -----------------------------------
		return ((ieee754.u & 0x7fffffff) > 0x7f800000);
#else
		return isnan(p_val);
#endif
	}

	static _ALWAYS_INLINE_ bool is_inf(double p_val) {
#ifdef _MSC_VER
		return !_finite(p_val);
// use an inline implementation of isinf as a workaround for problematic libstdc++ versions from gcc 5.x era
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint64_t u;
			double f;
		} ieee754;
		ieee754.f = p_val;
		return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
				((unsigned)ieee754.u == 0);
#else
		return isinf(p_val);
#endif
	}

	static _ALWAYS_INLINE_ bool is_inf(float p_val) {
#ifdef _MSC_VER
		return !_finite(p_val);
// use an inline implementation of isinf as a workaround for problematic libstdc++ versions from gcc 5.x era
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint32_t u;
			float f;
		} ieee754;
		ieee754.f = p_val;
		return (ieee754.u & 0x7fffffff) == 0x7f800000;
#else
		return isinf(p_val);
#endif
	}

	// These methods assume (p_num + p_den) doesn't overflow.
	static _ALWAYS_INLINE_ int32_t division_round_up(int32_t p_num, int32_t p_den) {
		int32_t offset = (p_num < 0 && p_den < 0) ? 1 : -1;
		return (p_num + p_den + offset) / p_den;
	}
	static _ALWAYS_INLINE_ uint32_t division_round_up(uint32_t p_num, uint32_t p_den) {
		return (p_num + p_den - 1) / p_den;
	}
	static _ALWAYS_INLINE_ int64_t division_round_up(int64_t p_num, int64_t p_den) {
		int32_t offset = (p_num < 0 && p_den < 0) ? 1 : -1;
		return (p_num + p_den + offset) / p_den;
	}
	static _ALWAYS_INLINE_ uint64_t division_round_up(uint64_t p_num, uint64_t p_den) {
		return (p_num + p_den - 1) / p_den;
	}

	static _ALWAYS_INLINE_ bool is_finite(double p_val) { return isfinite(p_val); }
	static _ALWAYS_INLINE_ bool is_finite(float p_val) { return isfinite(p_val); }

	static _ALWAYS_INLINE_ double abs(double g) { return absd(g); }
	static _ALWAYS_INLINE_ float abs(float g) { return absf(g); }
	static _ALWAYS_INLINE_ int abs(int g) { return g > 0 ? g : -g; }

	static _ALWAYS_INLINE_ double fposmod(double p_x, double p_y) {
		double value = Math::fmod(p_x, p_y);
		if (((value < 0) && (p_y > 0)) || ((value > 0) && (p_y < 0))) {
			value += p_y;
		}
		value += 0.0;
		return value;
	}
	static _ALWAYS_INLINE_ float fposmod(float p_x, float p_y) {
		float value = Math::fmod(p_x, p_y);
		if (((value < 0) && (p_y > 0)) || ((value > 0) && (p_y < 0))) {
			value += p_y;
		}
		value += 0.0f;
		return value;
	}
	static _ALWAYS_INLINE_ float fposmodp(float p_x, float p_y) {
		float value = Math::fmod(p_x, p_y);
		if (value < 0) {
			value += p_y;
		}
		value += 0.0f;
		return value;
	}
	static _ALWAYS_INLINE_ double fposmodp(double p_x, double p_y) {
		double value = Math::fmod(p_x, p_y);
		if (value < 0) {
			value += p_y;
		}
		value += 0.0;
		return value;
	}

	static _ALWAYS_INLINE_ int64_t posmod(int64_t p_x, int64_t p_y) {
		ERR_FAIL_COND_V_MSG(p_y == 0, 0, "Division by zero in posmod is undefined. Returning 0 as fallback.");
		int64_t value = p_x % p_y;
		if (((value < 0) && (p_y > 0)) || ((value > 0) && (p_y < 0))) {
			value += p_y;
		}
		return value;
	}

	static _ALWAYS_INLINE_ double deg_to_rad(double p_y) { return p_y * (Math_tau_over_2 / 180.0); }
	static _ALWAYS_INLINE_ float deg_to_rad(float p_y) { return p_y * (float)(Math_tau_over_2 / 180.0); }

	static _ALWAYS_INLINE_ double rad_to_deg(double p_y) { return p_y * (180.0 / Math_tau_over_2); }
	static _ALWAYS_INLINE_ float rad_to_deg(float p_y) { return p_y * (float)(180.0 / Math_tau_over_2); }

	static _ALWAYS_INLINE_ double lerp(double p_from, double p_to, double p_weight) { return p_from + (p_to - p_from) * p_weight; }
	static _ALWAYS_INLINE_ float lerp(float p_from, float p_to, float p_weight) { return p_from + (p_to - p_from) * p_weight; }

	static _ALWAYS_INLINE_ double cubic_interpolate(double p_from, double p_to, double p_pre, double p_post, double p_weight) {
		return 0.5 *
				((p_from * 2.0) +
						(-p_pre + p_to) * p_weight +
						(2.0 * p_pre - 5.0 * p_from + 4.0 * p_to - p_post) * (p_weight * p_weight) +
						(-p_pre + 3.0 * p_from - 3.0 * p_to + p_post) * (p_weight * p_weight * p_weight));
	}
	static _ALWAYS_INLINE_ float cubic_interpolate(float p_from, float p_to, float p_pre, float p_post, float p_weight) {
		return 0.5f *
				((p_from * 2.0f) +
						(-p_pre + p_to) * p_weight +
						(2.0f * p_pre - 5.0f * p_from + 4.0f * p_to - p_post) * (p_weight * p_weight) +
						(-p_pre + 3.0f * p_from - 3.0f * p_to + p_post) * (p_weight * p_weight * p_weight));
	}

	static _ALWAYS_INLINE_ double cubic_interpolate_angle(double p_from, double p_to, double p_pre, double p_post, double p_weight) {
		double from_rot = fmod(p_from, (double)Math_tau);

		double pre_diff = fmod(p_pre - from_rot, (double)Math_tau);
		double pre_rot = from_rot + fmod(2.0 * pre_diff, (double)Math_tau) - pre_diff;

		double to_diff = fmod(p_to - from_rot, (double)Math_tau);
		double to_rot = from_rot + fmod(2.0 * to_diff, (double)Math_tau) - to_diff;

		double post_diff = fmod(p_post - to_rot, (double)Math_tau);
		double post_rot = to_rot + fmod(2.0 * post_diff, (double)Math_tau) - post_diff;

		return cubic_interpolate(from_rot, to_rot, pre_rot, post_rot, p_weight);
	}

	static _ALWAYS_INLINE_ float cubic_interpolate_angle(float p_from, float p_to, float p_pre, float p_post, float p_weight) {
		float from_rot = fmod(p_from, Math_tau);

		float pre_diff = fmod(p_pre - from_rot, Math_tau);
		float pre_rot = from_rot + fmod(2.0f * pre_diff, Math_tau) - pre_diff;

		float to_diff = fmod(p_to - from_rot, Math_tau);
		float to_rot = from_rot + fmod(2.0f * to_diff, Math_tau) - to_diff;

		float post_diff = fmod(p_post - to_rot, Math_tau);
		float post_rot = to_rot + fmod(2.0f * post_diff, Math_tau) - post_diff;

		return cubic_interpolate(from_rot, to_rot, pre_rot, post_rot, p_weight);
	}

	static _ALWAYS_INLINE_ double cubic_interpolate_in_time(double p_from, double p_to, double p_pre, double p_post, double p_weight,
			double p_to_t, double p_pre_t, double p_post_t) {
		/* Barry-Goldman method */
		double t = Math::lerp(0.0, p_to_t, p_weight);
		double a1 = Math::lerp(p_pre, p_from, p_pre_t == 0 ? 0.0 : (t - p_pre_t) / -p_pre_t);
		double a2 = Math::lerp(p_from, p_to, p_to_t == 0 ? 0.5 : t / p_to_t);
		double a3 = Math::lerp(p_to, p_post, p_post_t - p_to_t == 0 ? 1.0 : (t - p_to_t) / (p_post_t - p_to_t));
		double b1 = Math::lerp(a1, a2, p_to_t - p_pre_t == 0 ? 0.0 : (t - p_pre_t) / (p_to_t - p_pre_t));
		double b2 = Math::lerp(a2, a3, p_post_t == 0 ? 1.0 : t / p_post_t);
		return Math::lerp(b1, b2, p_to_t == 0 ? 0.5 : t / p_to_t);
	}

	static _ALWAYS_INLINE_ float cubic_interpolate_in_time(float p_from, float p_to, float p_pre, float p_post, float p_weight,
			float p_to_t, float p_pre_t, float p_post_t) {
		/* Barry-Goldman method */
		float t = Math::lerp(0.0f, p_to_t, p_weight);
		float a1 = Math::lerp(p_pre, p_from, p_pre_t == 0 ? 0.0f : (t - p_pre_t) / -p_pre_t);
		float a2 = Math::lerp(p_from, p_to, p_to_t == 0 ? 0.5f : t / p_to_t);
		float a3 = Math::lerp(p_to, p_post, p_post_t - p_to_t == 0 ? 1.0f : (t - p_to_t) / (p_post_t - p_to_t));
		float b1 = Math::lerp(a1, a2, p_to_t - p_pre_t == 0 ? 0.0f : (t - p_pre_t) / (p_to_t - p_pre_t));
		float b2 = Math::lerp(a2, a3, p_post_t == 0 ? 1.0f : t / p_post_t);
		return Math::lerp(b1, b2, p_to_t == 0 ? 0.5f : t / p_to_t);
	}

	static _ALWAYS_INLINE_ double cubic_interpolate_angle_in_time(double p_from, double p_to, double p_pre, double p_post, double p_weight,
			double p_to_t, double p_pre_t, double p_post_t) {
		double from_rot = fmod(p_from, (double)Math_tau);

		double pre_diff = fmod(p_pre - from_rot, (double)Math_tau);
		double pre_rot = from_rot + fmod(2.0 * pre_diff, (double)Math_tau) - pre_diff;

		double to_diff = fmod(p_to - from_rot, (double)Math_tau);
		double to_rot = from_rot + fmod(2.0 * to_diff, (double)Math_tau) - to_diff;

		double post_diff = fmod(p_post - to_rot, (double)Math_tau);
		double post_rot = to_rot + fmod(2.0 * post_diff, (double)Math_tau) - post_diff;

		return cubic_interpolate_in_time(from_rot, to_rot, pre_rot, post_rot, p_weight, p_to_t, p_pre_t, p_post_t);
	}

	static _ALWAYS_INLINE_ float cubic_interpolate_angle_in_time(float p_from, float p_to, float p_pre, float p_post, float p_weight,
			float p_to_t, float p_pre_t, float p_post_t) {
		float from_rot = fmod(p_from, Math_tau);

		float pre_diff = fmod(p_pre - from_rot, Math_tau);
		float pre_rot = from_rot + fmod(2.0f * pre_diff, Math_tau) - pre_diff;

		float to_diff = fmod(p_to - from_rot, Math_tau);
		float to_rot = from_rot + fmod(2.0f * to_diff, Math_tau) - to_diff;

		float post_diff = fmod(p_post - to_rot, Math_tau);
		float post_rot = to_rot + fmod(2.0f * post_diff, Math_tau) - post_diff;

		return cubic_interpolate_in_time(from_rot, to_rot, pre_rot, post_rot, p_weight, p_to_t, p_pre_t, p_post_t);
	}

	static _ALWAYS_INLINE_ double bezier_interpolate(double p_start, double p_control_1, double p_control_2, double p_end, double p_t) {
		/* Formula from Wikipedia article on Bezier curves. */
		double omt = (1.0 - p_t);
		double omt2 = omt * omt;
		double omt3 = omt2 * omt;
		double t2 = p_t * p_t;
		double t3 = t2 * p_t;

		return p_start * omt3 + p_control_1 * omt2 * p_t * 3.0 + p_control_2 * omt * t2 * 3.0 + p_end * t3;
	}

	static _ALWAYS_INLINE_ float bezier_interpolate(float p_start, float p_control_1, float p_control_2, float p_end, float p_t) {
		/* Formula from Wikipedia article on Bezier curves. */
		float omt = (1.0f - p_t);
		float omt2 = omt * omt;
		float omt3 = omt2 * omt;
		float t2 = p_t * p_t;
		float t3 = t2 * p_t;

		return p_start * omt3 + p_control_1 * omt2 * p_t * 3.0f + p_control_2 * omt * t2 * 3.0f + p_end * t3;
	}

	static _ALWAYS_INLINE_ double bezier_derivative(double p_start, double p_control_1, double p_control_2, double p_end, double p_t) {
		/* Formula from Wikipedia article on Bezier curves. */
		double omt = (1.0 - p_t);
		double omt2 = omt * omt;
		double t2 = p_t * p_t;

		double d = (p_control_1 - p_start) * 3.0 * omt2 + (p_control_2 - p_control_1) * 6.0 * omt * p_t + (p_end - p_control_2) * 3.0 * t2;
		return d;
	}

	static _ALWAYS_INLINE_ float bezier_derivative(float p_start, float p_control_1, float p_control_2, float p_end, float p_t) {
		/* Formula from Wikipedia article on Bezier curves. */
		float omt = (1.0f - p_t);
		float omt2 = omt * omt;
		float t2 = p_t * p_t;

		float d = (p_control_1 - p_start) * 3.0f * omt2 + (p_control_2 - p_control_1) * 6.0f * omt * p_t + (p_end - p_control_2) * 3.0f * t2;
		return d;
	}

	static _ALWAYS_INLINE_ double angle_difference(double p_from, double p_to) {
		double difference = fmod(p_to - p_from, (double)Math_tau);
		return fmod(2.0 * difference, (double)Math_tau) - difference;
	}
	static _ALWAYS_INLINE_ float angle_difference(float p_from, float p_to) {
		float difference = fmod(p_to - p_from, Math_tau);
		return fmod(2.0f * difference, Math_tau) - difference;
	}

	static _ALWAYS_INLINE_ double lerp_angle(double p_from, double p_to, double p_weight) {
		return p_from + Math::angle_difference(p_from, p_to) * p_weight;
	}
	static _ALWAYS_INLINE_ float lerp_angle(float p_from, float p_to, float p_weight) {
		return p_from + Math::angle_difference(p_from, p_to) * p_weight;
	}

	static _ALWAYS_INLINE_ double inverse_lerp(double p_from, double p_to, double p_value) {
		return (p_value - p_from) / (p_to - p_from);
	}
	static _ALWAYS_INLINE_ float inverse_lerp(float p_from, float p_to, float p_value) {
		return (p_value - p_from) / (p_to - p_from);
	}

	static _ALWAYS_INLINE_ double remap(double p_value, double p_istart, double p_istop, double p_ostart, double p_ostop) {
		return Math::lerp(p_ostart, p_ostop, Math::inverse_lerp(p_istart, p_istop, p_value));
	}
	static _ALWAYS_INLINE_ float remap(float p_value, float p_istart, float p_istop, float p_ostart, float p_ostop) {
		return Math::lerp(p_ostart, p_ostop, Math::inverse_lerp(p_istart, p_istop, p_value));
	}

	static _ALWAYS_INLINE_ double smoothstep(double p_from, double p_to, double p_s) {
		if (is_equal_approx(p_from, p_to)) {
			return p_from;
		}
		double s = CLAMP((p_s - p_from) / (p_to - p_from), 0.0, 1.0);
		return s * s * (3.0 - 2.0 * s);
	}
	static _ALWAYS_INLINE_ float smoothstep(float p_from, float p_to, float p_s) {
		if (is_equal_approx(p_from, p_to)) {
			return p_from;
		}
		float s = CLAMP((p_s - p_from) / (p_to - p_from), 0.0f, 1.0f);
		return s * s * (3.0f - 2.0f * s);
	}

	static _ALWAYS_INLINE_ double move_toward(double p_from, double p_to, double p_delta) {
		return abs(p_to - p_from) <= p_delta ? p_to : p_from + SIGN(p_to - p_from) * p_delta;
	}
	static _ALWAYS_INLINE_ float move_toward(float p_from, float p_to, float p_delta) {
		return abs(p_to - p_from) <= p_delta ? p_to : p_from + SIGN(p_to - p_from) * p_delta;
	}

	static _ALWAYS_INLINE_ double rotate_toward(double p_from, double p_to, double p_delta) {
		double difference = Math::angle_difference(p_from, p_to);
		double abs_difference = Math::abs(difference);
		// When `p_delta < 0` move no further than to PI radians away from `p_to` (as PI is the max possible angle distance).
		return p_from + CLAMP(p_delta, abs_difference - Math_tau_over_2, abs_difference) * (difference >= 0.0 ? 1.0 : -1.0);
	}
	static _ALWAYS_INLINE_ float rotate_toward(float p_from, float p_to, float p_delta) {
		float difference = Math::angle_difference(p_from, p_to);
		float abs_difference = Math::abs(difference);
		// When `p_delta < 0` move no further than to PI radians away from `p_to` (as PI is the max possible angle distance).
		return p_from + CLAMP(p_delta, abs_difference - Math_tau_over_2, abs_difference) * (difference >= 0.0f ? 1.0f : -1.0f);
	}

	static _ALWAYS_INLINE_ double linear_to_db(double p_linear) {
		return Math::log(p_linear) * 8.6858896380650365530225783783321;
	}
	static _ALWAYS_INLINE_ float linear_to_db(float p_linear) {
		return Math::log(p_linear) * (float)8.6858896380650365530225783783321;
	}

	static _ALWAYS_INLINE_ double db_to_linear(double p_db) {
		return Math::exp(p_db * 0.11512925464970228420089957273422);
	}
	static _ALWAYS_INLINE_ float db_to_linear(float p_db) {
		return Math::exp(p_db * (float)0.11512925464970228420089957273422);
	}

	static _ALWAYS_INLINE_ double round(double p_val) { return ::round(p_val); }
	static _ALWAYS_INLINE_ float round(float p_val) { return ::roundf(p_val); }

	static _ALWAYS_INLINE_ int64_t wrapi(int64_t value, int64_t min, int64_t max) {
		int64_t range = max - min;
		return range == 0 ? min : min + ((((value - min) % range) + range) % range);
	}
	static _ALWAYS_INLINE_ double wrapf(double value, double min, double max) {
		double range = max - min;
		if (is_zero_approx(range)) {
			return min;
		}
		double result = value - (range * Math::floor((value - min) / range));
		if (is_equal_approx(result, max)) {
			return min;
		}
		return result;
	}
	static _ALWAYS_INLINE_ float wrapf(float value, float min, float max) {
		float range = max - min;
		if (is_zero_approx(range)) {
			return min;
		}
		float result = value - (range * Math::floor((value - min) / range));
		if (is_equal_approx(result, max)) {
			return min;
		}
		return result;
	}

	static _ALWAYS_INLINE_ float fract(float value) {
		return value - floor(value);
	}
	static _ALWAYS_INLINE_ double fract(double value) {
		return value - floor(value);
	}
	static _ALWAYS_INLINE_ float pingpong(float value, float length) {
		return (length != 0.0f) ? abs(fract((value - length) / (length * 2.0f)) * length * 2.0f - length) : 0.0f;
	}
	static _ALWAYS_INLINE_ double pingpong(double value, double length) {
		return (length != 0.0) ? abs(fract((value - length) / (length * 2.0)) * length * 2.0 - length) : 0.0;
	}

	// double only, as these functions are mainly used by the editor and not performance-critical,
	static double ease(double p_x, double p_c);
	static int step_decimals(double p_step);
	static int range_step_decimals(double p_step); // For editor use only.
	static double snapped(double p_value, double p_step);

	static uint32_t larger_prime(uint32_t p_val);

	static void seed(uint64_t x);
	static void randomize();
	static uint32_t rand_from_seed(uint64_t *seed);
	static uint32_t rand();
	static _ALWAYS_INLINE_ double randd() { return (double)rand() / (double)Math::RANDOM_32BIT_MAX; }
	static _ALWAYS_INLINE_ float randf() { return (float)rand() / (float)Math::RANDOM_32BIT_MAX; }
	static double randfn(double mean, double deviation);

	static double random(double from, double to);
	static float random(float from, float to);
	static int random(int from, int to);

	static _ALWAYS_INLINE_ bool is_equal_approx(float a, float b) {
		// Check for exact equality first, required to handle "infinity" values.
		if (a == b) {
			return true;
		}
		// Then check for approximate equality.
		float tolerance = (float)CMP_EPSILON * abs(a);
		if (tolerance < (float)CMP_EPSILON) {
			tolerance = (float)CMP_EPSILON;
		}
		return abs(a - b) < tolerance;
	}

	static _ALWAYS_INLINE_ bool is_equal_approx(float a, float b, float tolerance) {
		// Check for exact equality first, required to handle "infinity" values.
		if (a == b) {
			return true;
		}
		// Then check for approximate equality.
		return abs(a - b) < tolerance;
	}

	static _ALWAYS_INLINE_ bool is_zero_approx(float s) {
		return abs(s) < (float)CMP_EPSILON;
	}

	static _ALWAYS_INLINE_ bool is_equal_approx(double a, double b) {
		// Check for exact equality first, required to handle "infinity" values.
		if (a == b) {
			return true;
		}
		// Then check for approximate equality.
		double tolerance = CMP_EPSILON * abs(a);
		if (tolerance < CMP_EPSILON) {
			tolerance = CMP_EPSILON;
		}
		return abs(a - b) < tolerance;
	}

	static _ALWAYS_INLINE_ bool is_equal_approx(double a, double b, double tolerance) {
		// Check for exact equality first, required to handle "infinity" values.
		if (a == b) {
			return true;
		}
		// Then check for approximate equality.
		return abs(a - b) < tolerance;
	}

	static _ALWAYS_INLINE_ bool is_zero_approx(double s) {
		return abs(s) < CMP_EPSILON;
	}

	static _ALWAYS_INLINE_ float absf(float g) {
		union {
			float f;
			uint32_t i;
		} u;

		u.f = g;
		u.i &= 2147483647u;
		return u.f;
	}

	static _ALWAYS_INLINE_ double absd(double g) {
		union {
			double d;
			uint64_t i;
		} u;
		u.d = g;
		u.i &= (uint64_t)9223372036854775807ll;
		return u.d;
	}

	// This function should be as fast as possible and rounding mode should not matter.
	static _ALWAYS_INLINE_ int fast_ftoi(float a) {
		// Assuming every supported compiler has `lrint()`.
		return lrintf(a);
	}

	static _ALWAYS_INLINE_ uint32_t halfbits_to_floatbits(uint16_t h) {
		uint16_t h_exp, h_sig;
		uint32_t f_sgn, f_exp, f_sig;

		h_exp = (h & 0x7c00u);
		f_sgn = ((uint32_t)h & 0x8000u) << 16;
		switch (h_exp) {
			case 0x0000u: /* 0 or subnormal */
				h_sig = (h & 0x03ffu);
				/* Signed zero */
				if (h_sig == 0) {
					return f_sgn;
				}
				/* Subnormal */
				h_sig <<= 1;
				while ((h_sig & 0x0400u) == 0) {
					h_sig <<= 1;
					h_exp++;
				}
				f_exp = ((uint32_t)(127 - 15 - h_exp)) << 23;
				f_sig = ((uint32_t)(h_sig & 0x03ffu)) << 13;
				return f_sgn + f_exp + f_sig;
			case 0x7c00u: /* inf or NaN */
				/* All-ones exponent and a copy of the significand */
				return f_sgn + 0x7f800000u + (((uint32_t)(h & 0x03ffu)) << 13);
			default: /* normalized */
				/* Just need to adjust the exponent and shift */
				return f_sgn + (((uint32_t)(h & 0x7fffu) + 0x1c000u) << 13);
		}
	}

	static _ALWAYS_INLINE_ float halfptr_to_float(const uint16_t *h) {
		union {
			uint32_t u32;
			float f32;
		} u;

		u.u32 = halfbits_to_floatbits(*h);
		return u.f32;
	}

	static _ALWAYS_INLINE_ float half_to_float(const uint16_t h) {
		return halfptr_to_float(&h);
	}

	static _ALWAYS_INLINE_ uint16_t make_half_float(float f) {
		union {
			float fv;
			uint32_t ui;
		} ci;
		ci.fv = f;

		uint32_t x = ci.ui;
		uint32_t sign = (unsigned short)(x >> 31);
		uint32_t mantissa;
		uint32_t exponent;
		uint16_t hf;

		// get mantissa
		mantissa = x & ((1 << 23) - 1);
		// get exponent bits
		exponent = x & (0xFF << 23);
		if (exponent >= 0x47800000) {
			// check if the original single precision float number is a NaN
			if (mantissa && (exponent == (0xFF << 23))) {
				// we have a single precision NaN
				mantissa = (1 << 23) - 1;
			} else {
				// 16-bit half-float representation stores number as Inf
				mantissa = 0;
			}
			hf = (((uint16_t)sign) << 15) | (uint16_t)((0x1F << 10)) |
					(uint16_t)(mantissa >> 13);
		}
		// check if exponent is <= -15
		else if (exponent <= 0x38000000) {
			/*
			// store a denorm half-float value or zero
			exponent = (0x38000000 - exponent) >> 23;
			mantissa >>= (14 + exponent);

			hf = (((uint16_t)sign) << 15) | (uint16_t)(mantissa);
			*/
			hf = 0; //denormals do not work for 3D, convert to zero
		} else {
			hf = (((uint16_t)sign) << 15) |
					(uint16_t)((exponent - 0x38000000) >> 13) |
					(uint16_t)(mantissa >> 13);
		}

		return hf;
	}

	static _ALWAYS_INLINE_ float snap_scalar(float p_offset, float p_step, float p_target) {
		return p_step != 0 ? Math::snapped(p_target - p_offset, p_step) + p_offset : p_target;
	}

	static _ALWAYS_INLINE_ float snap_scalar_separation(float p_offset, float p_step, float p_target, float p_separation) {
		if (p_step != 0) {
			float a = Math::snapped(p_target - p_offset, p_step + p_separation) + p_offset;
			float b = a;
			if (p_target >= 0) {
				b -= p_separation;
			} else {
				b += p_step;
			}
			return (Math::abs(p_target - a) < Math::abs(p_target - b)) ? a : b;
		}
		return p_target;
	}
};

#endif // MATH_FUNCS_H
