#ifndef EAE6320_UTILS_H
#define EAE6320_UTILS_H

namespace eae6320
{
	namespace Utils
	{
		bool ReadDataFromFile(const char* const i_path, void** i_temporaryBuffer);
		float GetLambda(float i_a, float i_b, float i_y);
		float sgn(float i_a);

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

		struct FloatPointUtils
		{
		public:
			//float comparision using max difference and max relative difference.
			static bool AlmostEqualRelativeAndAbs(float A, float B,
				float maxDiff, float maxRelDiff);
			//float comparision using max difference and max ulp difference
			static bool AlmostEqualUlpsAndAbs(float A, float B,
				float maxDiff, int maxUlpsDiff);

			static bool isNaN(const float i_val);

			static bool isInfinity(const float i_val);

			static bool isNaNOrInfinity(const float i_val);

		};
	}
}

#endif	// EAE6320_UTILS_H
