#include "MathFloat.h"

#include "FloatPointUtils.h"

#include <cassert>

float eae6320::Math::MathFloat::GetLambda(float i_a, float i_b, float i_y)
{
	assert(FloatPointUtils::AlmostEqualRelativeAndAbs(i_a, i_b));
	return (i_y - i_a) / (i_b - i_a);
}
float eae6320::Math::MathFloat::sgn(float i_a)
{
	return static_cast<float>((0.0f < i_a) - (i_a < 0.0f));
}
