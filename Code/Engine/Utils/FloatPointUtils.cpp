#include "FloatPointUtils.h"

bool eae6320::Utils::FloatPointUtils::AlmostEqualRelativeAndAbs(float A, float B,
	float maxDiff, float maxRelDiff)
{
	// Check if the numbers are really close -- needed
	// when comparing numbers near zero.
	float diff = fabs(A - B);
	if (diff <= maxDiff)
		return true;

	A = fabs(A);
	B = fabs(B);
	float largest = (B > A) ? B : A;

	//check if the numbers are really close in relative -- works when comparing numbers far from zero.
	if (diff <= largest * maxRelDiff)
		return true;
	return false;
}
bool eae6320::Utils::FloatPointUtils::AlmostEqualUlpsAndAbs(float A, float B,
	float maxDiff, int maxUlpsDiff)
{
	// Check if the numbers are really close -- needed
	// when comparing numbers near zero.
	float absDiff = fabs(A - B);
	if (absDiff <= maxDiff)
		return true;

	Float_t uA(A);
	Float_t uB(B);

	// Different signs means they do not match.
	if (uA.Negative() != uB.Negative())
		return false;

	// Find the difference in ULPs.
	int ulpsDiff = abs(uA.i - uB.i);
	if (ulpsDiff <= maxUlpsDiff)
		return true;

	return false;
}
bool eae6320::Utils::FloatPointUtils::isNaN(const float i_val)
{
	volatile float  v = i_val;
	return v != v;
}
bool eae6320::Utils::FloatPointUtils::isInfinity(const float i_val)
{
	volatile float v = i_val;

	return ((v * 0) != 0) && (v == v);
}

bool eae6320::Utils::FloatPointUtils::isNaNOrInfinity(const float i_val)
{
	volatile float v = i_val;

	return (v * 0) != 0;
}