// Header Files
//=============

#include "cVector4.h"

#include <cassert>
#include <cmath>

// Static Data Initialization
//===========================

namespace
{
	const float s_epsilon = 1.0e-9f;
}

// Interface
//==========

// Addition
eae6320::Math::cVector4 eae6320::Math::cVector4::operator +(const cVector4& i_rhs) const
{
	return cVector4(x + i_rhs.x, y + i_rhs.y, z + i_rhs.z, w + i_rhs.w);
}
eae6320::Math::cVector4& eae6320::Math::cVector4::operator +=(const cVector4& i_rhs)
{
	x += i_rhs.x;
	y += i_rhs.y;
	z += i_rhs.z;
	w += i_rhs.w;
	return *this;
}

// Subtraction / Negation
eae6320::Math::cVector4 eae6320::Math::cVector4::operator -(const cVector4& i_rhs) const
{
	return cVector4(x - i_rhs.x, y - i_rhs.y, z - i_rhs.z, w - i_rhs.w);
}
eae6320::Math::cVector4& eae6320::Math::cVector4::operator -=(const cVector4& i_rhs)
{
	x -= i_rhs.x;
	y -= i_rhs.y;
	z -= i_rhs.z;
	w -= i_rhs.w;
	return *this;
}
eae6320::Math::cVector4 eae6320::Math::cVector4::operator -() const
{
	return cVector4(-x, -y, -z, -w);
}

// Multiplication
eae6320::Math::cVector4 eae6320::Math::cVector4::operator *(const float i_rhs) const
{
	return cVector4(x * i_rhs, y * i_rhs, z * i_rhs, w * i_rhs);
}
eae6320::Math::cVector4& eae6320::Math::cVector4::operator *=(const float i_rhs)
{
	x *= i_rhs;
	y *= i_rhs;
	z *= i_rhs;
	w *= i_rhs;
	return *this;
}
eae6320::Math::cVector4 operator *(const float i_lhs, const eae6320::Math::cVector4& i_rhs)
{
	return i_rhs * i_lhs;
}

// Division
eae6320::Math::cVector4 eae6320::Math::cVector4::operator /(const float i_rhs) const
{
	assert(std::abs(i_rhs) > s_epsilon);
	const float rhs_reciprocal = 1.0f / i_rhs;
	return cVector4(x * rhs_reciprocal, y * rhs_reciprocal, z * rhs_reciprocal, w * rhs_reciprocal);
}
eae6320::Math::cVector4& eae6320::Math::cVector4::operator /=(const float i_rhs)
{
	assert(std::abs(i_rhs) > s_epsilon);
	const float rhs_reciprocal = 1.0f / i_rhs;
	x *= rhs_reciprocal;
	y *= rhs_reciprocal;
	z *= rhs_reciprocal;
	w *= rhs_reciprocal;
	return *this;
}

// Length / Normalization
float eae6320::Math::cVector4::GetLength() const
{
	return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
}
float eae6320::Math::cVector4::Normalize()
{
	const float length = GetLength();
	assert(length > s_epsilon);
	operator /=(length);
	return length;
}
eae6320::Math::cVector4 eae6320::Math::cVector4::CreateNormalized() const
{
	const float length = GetLength();
	assert(length > s_epsilon);
	const float length_reciprocal = 1.0f / length;
	return cVector4(x * length_reciprocal, y * length_reciprocal, z * length_reciprocal, w * length_reciprocal);
}

// Products
float eae6320::Math::Dot(const cVector4& i_lhs, const cVector4& i_rhs)
{
	return (i_lhs.x * i_rhs.x) + (i_lhs.y * i_rhs.y) + (i_lhs.z * i_rhs.z) + (i_lhs.w * i_rhs.w);
}

// Comparison
bool eae6320::Math::cVector4::operator ==(const cVector4& i_rhs) const
{
	// Use & rather than && to prevent branches (all three comparisons will be evaluated)
	return (x == i_rhs.x) & (y == i_rhs.y) & (z == i_rhs.z) & (w == i_rhs.w);
}
bool eae6320::Math::cVector4::operator !=(const cVector4& i_rhs) const
{
	// Use | rather than || to prevent branches (all three comparisons will be evaluated)
	return (x != i_rhs.x) | (y != i_rhs.y) | (z != i_rhs.z) | (w != i_rhs.w);
}

// Initialization / Shut Down
//---------------------------

eae6320::Math::cVector4::cVector4(const float i_x, const float i_y, const float i_z, const float i_w)
	:
	x(i_x), y(i_y), z(i_z), w(i_w)
{

}

eae6320::Math::cVector4::cVector4(const cVector& i_vector, const float i_w)
	:
	x(i_vector.x), y(i_vector.y), z(i_vector.z), w(i_w)
{

}
