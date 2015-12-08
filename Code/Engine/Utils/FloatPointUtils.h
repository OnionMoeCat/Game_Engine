#ifndef EAE6320_FLOATPOINTUTILS_H
#define EAE6320_FLOATPOINTUTILS_H

#include <stdint.h>
#include <cmath>
#include <cfloat>

namespace eae6320
{
	namespace Utils
	{
		//ulp representation of float
		union Float_t
		{
			Float_t(float num = 0.0f) : f(num) {}
			// Portable extraction of components.
			bool Negative() const { return (i >> 31) != 0; }
			int32_t RawMantissa() const { return i & ((1 << 23) - 1); }
			int32_t RawExponent() const { return (i >> 23) & 0xFF; }

			int32_t i;
			float f;
#ifdef _DEBUG
			struct
			{   // Bitfields for exploration. Do not use in production code.
				uint32_t mantissa : 23;
				uint32_t exponent : 8;
				uint32_t sign : 1;
			} parts;
#endif
		};

		namespace FloatPointUtils
		{
			//float comparision using max difference and max relative difference.
			bool AlmostEqualRelativeAndAbs(float A, float B,
				float maxDiff = 4 * FLT_EPSILON, float maxRelDiff = 4 * FLT_EPSILON);
			//float comparision using max difference and max ulp difference
			bool AlmostEqualUlpsAndAbs(float A, float B,
				float maxDiff, int maxUlpsDiff);

			bool isNaN(const float i_val);

			bool isInfinity(const float i_val);

			bool isNaNOrInfinity(const float i_val);

		}
	}
}
#endif