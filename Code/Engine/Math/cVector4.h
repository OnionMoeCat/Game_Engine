/*
This class represents a position or direction
*/

#ifndef EAE6320_MATH_CVECTOR4_H
#define EAE6320_MATH_CVECTOR4_H

#include "cVector.h"

// Class Declaration
//==================

namespace eae6320
{
	namespace Math
	{
		class cVector4
		{
			// Interface
			//==========

		public:

			// Addition
			cVector4 operator +(const cVector4& i_rhs) const;
			cVector4& operator +=(const cVector4& i_rhs);

			// Subtraction / Negation
			cVector4 operator -(const cVector4& i_rhs) const;
			cVector4& operator -=(const cVector4& i_rhs);
			cVector4 operator -() const;

			// Multiplication
			cVector4 operator *(const float i_rhs) const;
			cVector4& operator *=(const float i_rhs);
			friend cVector4 operator *(const float i_lhs, const cVector4& i_rhs);

			// Division
			cVector4 operator /(const float i_rhs) const;
			cVector4& operator /=(const float i_rhs);

			// Length / Normalization
			float GetLength() const;
			float Normalize();
			cVector4 CreateNormalized() const;

			// Products
			friend float Dot(const cVector4& i_lhs, const cVector4& i_rhs);

			// Comparison
			bool operator ==(const cVector4& i_rhs) const;
			bool operator !=(const cVector4& i_rhs) const;

			// Initialization / Shut Down
			//---------------------------

			cVector4(const float i_x = 0.0f, const float i_y = 0.0f, const float i_z = 0.0f, const float i_w = 1.0f);
			cVector4(const cVector& i_vector, const float i_w = 1.0f);

			// Data
			//=====

			// A vector class is one of the very few examples where I make data public
		public:

			float x, y, z, w;

			// Friend Classes
			//===============

			friend class cMatrix_transformation;
		};

		// Friend Function Declarations
		//=============================

		cVector4 operator *(const float i_lhs, const cVector4& i_rhs);
		float Dot(const cVector4& i_lhs, const cVector4& i_rhs);
		cVector4 Cross(const cVector4& i_lhs, const cVector4& i_rhs);
	}
}

#endif	// EAE6320_MATH_CVECTOR_H
