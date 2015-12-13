// Header Files
//=============

#include "cMatrix_transformation.h"

#include <cmath>
#include <cassert>
#include "cQuaternion.h"
#include "cVector.h"
#include "FloatPointUtils.h"

// Interface
//==========

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(
	const cQuaternion& i_cameraOrientation, const cVector& i_cameraPosition )
{
	cMatrix_transformation transform_viewToWorld( i_cameraOrientation, i_cameraPosition );
	// A camera can only ever have rotation and translation
	// and so a lot of simplifying assumptions can be made in order to create the inverse
	return cMatrix_transformation(
		transform_viewToWorld.m_00, transform_viewToWorld.m_01, transform_viewToWorld.m_02,
			-( transform_viewToWorld.m_30 * transform_viewToWorld.m_00 ) - ( transform_viewToWorld.m_31 * transform_viewToWorld.m_01 ) - ( transform_viewToWorld.m_32 * transform_viewToWorld.m_02 ),
		transform_viewToWorld.m_10, transform_viewToWorld.m_11, transform_viewToWorld.m_12,
			-( transform_viewToWorld.m_30 * transform_viewToWorld.m_10 ) - ( transform_viewToWorld.m_31 * transform_viewToWorld.m_11 ) - ( transform_viewToWorld.m_32 * transform_viewToWorld.m_12 ),
		transform_viewToWorld.m_20, transform_viewToWorld.m_21, transform_viewToWorld.m_22,
			-( transform_viewToWorld.m_30 * transform_viewToWorld.m_20 ) - ( transform_viewToWorld.m_31 * transform_viewToWorld.m_21 ) - ( transform_viewToWorld.m_32 * transform_viewToWorld.m_22 ),
		0.0f, 0.0f, 0.0f, 1.0f );
}

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateViewToScreenTransformPerspective(
	const float i_fieldOfView_y, const float i_aspectRatio,
	const float i_z_nearPlane, const float i_z_farPlane )
{
	const float yScale = 1.0f / std::tan( i_fieldOfView_y * 0.5f );
	const float xScale = yScale / i_aspectRatio;
#if defined( EAE6320_PLATFORM_D3D )
	const float zDistanceScale = i_z_farPlane / ( i_z_nearPlane - i_z_farPlane );
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zDistanceScale, i_z_nearPlane * zDistanceScale,
		0.0f, 0.0f, -1.0f, 0.0f );
#elif defined( EAE6320_PLATFORM_GL )
	const float zDistanceScale = 1.0f / ( i_z_nearPlane - i_z_farPlane );
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, ( i_z_nearPlane + i_z_farPlane ) * zDistanceScale, ( 2.0f * i_z_nearPlane * i_z_farPlane ) * zDistanceScale,
		0.0f, 0.0f, -1.0f, 0.0f );
#endif
}

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateViewToScreenTransformOrthographic(
	const float i_x_halfLength, const float i_y_halfLength,
	const float i_z_nearPlane, const float i_z_farPlane)
{
	const float yScale = 1.0f / i_y_halfLength;
	const float xScale = 1.0f / i_x_halfLength;
#if defined( EAE6320_PLATFORM_D3D )
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f / (i_z_farPlane - i_z_nearPlane), - i_z_nearPlane / (i_z_farPlane - i_z_nearPlane),
		0.0f, 0.0f, 0.0f, 1.0f);
#elif defined( EAE6320_PLATFORM_GL )
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, -2.0f / (i_z_farPlane - i_z_nearPlane), - (i_z_farPlane + i_z_nearPlane) / (i_z_farPlane - i_z_nearPlane),
		0.0f, 0.0f, 0.0f, 1.0f);
#endif
}

// Initialization / Shut Down
//---------------------------

eae6320::Math::cMatrix_transformation::cMatrix_transformation()
	:
	m_00( 1.0f ), m_10( 0.0f ), m_20( 0.0f ), m_30( 0.0f ),
	m_01( 0.0f ), m_11( 1.0f ), m_21( 0.0f ), m_31( 0.0f ),
	m_02( 0.0f ), m_12( 0.0f ), m_22( 1.0f ), m_32( 0.0f ),
	m_03( 0.0f ), m_13( 0.0f ), m_23( 0.0f ), m_33( 1.0f )
{

}

eae6320::Math::cMatrix_transformation::cMatrix_transformation( const cQuaternion& i_rotation, const cVector& i_translation )
	:
	m_30( i_translation.x ), m_31( i_translation.y ), m_32( i_translation.z ),
	m_03( 0.0f ), m_13( 0.0f ), m_23( 0.0f ), m_33( 1.0f )
{
	const float _2x = i_rotation.m_x + i_rotation.m_x;
	const float _2y = i_rotation.m_y + i_rotation.m_y;
	const float _2z = i_rotation.m_z + i_rotation.m_z;
	const float _2xx = i_rotation.m_x * _2x;
	const float _2xy = _2x * i_rotation.m_y;
	const float _2xz = _2x * i_rotation.m_z;
	const float _2xw = _2x * i_rotation.m_w;
	const float _2yy = _2y * i_rotation.m_y;
	const float _2yz = _2y * i_rotation.m_z;
	const float _2yw = _2y * i_rotation.m_w;
	const float _2zz = _2z * i_rotation.m_z;
	const float _2zw = _2z * i_rotation.m_w;

	m_00 = 1.0f - _2yy - _2zz;
	m_10 = _2xy + _2zw;
	m_20 = _2xz - _2yw;

	m_01 = _2xy - _2zw;
	m_11 = 1.0f - _2xx - _2zz;
	m_21 = _2yz + _2xw;

	m_02 = _2xz + _2yw;
	m_12 = _2yz - _2xw;
	m_22 = 1.0f - _2xx - _2yy;
}

// Implementation
//===============

// Initialization / Shut Down
//---------------------------

eae6320::Math::cMatrix_transformation::cMatrix_transformation(
	const float i_00, const float i_10, const float i_20, const float i_30,
	const float i_01, const float i_11, const float i_21, const float i_31,
	const float i_02, const float i_12, const float i_22, const float i_32,
	const float i_03, const float i_13, const float i_23, const float i_33 )
	:
	m_00( i_00 ), m_10( i_10 ), m_20( i_20 ), m_30( i_30 ),
	m_01( i_01 ), m_11( i_11 ), m_21( i_21 ), m_31( i_31 ),
	m_02( i_02 ), m_12( i_12 ), m_22( i_22 ), m_32( i_32 ),
	m_03( i_03 ), m_13( i_13 ), m_23( i_23 ), m_33( i_33 )
{

}

/*eae6320::Math::cVector eae6320::Math::operator*(const cVector& lhs, const cMatrix_transformation& rhs)
{
	float normalW = 1.0f;
	float t0 = lhs.x * rhs.m_00 + lhs.y * rhs.m_10 + lhs.z * rhs.m_20 + normalW * rhs.m_30;
	float t1 = lhs.x * rhs.m_01 + lhs.y * rhs.m_11 + lhs.z * rhs.m_21 + normalW * rhs.m_31;
	float t2 = lhs.x * rhs.m_02 + lhs.y * rhs.m_12 + lhs.z * rhs.m_22 + normalW * rhs.m_32;
	float t3 = lhs.x * rhs.m_03 + lhs.y * rhs.m_13 + lhs.z * rhs.m_23 + normalW * rhs.m_33;
	assert(!FloatPointUtils::AlmostEqualRelativeAndAbs(t3, 0.0f));
	return cVector(t0 / t3, t1 / t3, t2 / t3);
}*/

eae6320::Math::cVector4 eae6320::Math::operator*(const eae6320::Math::cVector4& lhs, const eae6320::Math::cMatrix_transformation& rhs)
{
	float t0 = lhs.x * rhs.m_00 + lhs.y * rhs.m_10 + lhs.z * rhs.m_20 + lhs.w * rhs.m_30;
	float t1 = lhs.x * rhs.m_01 + lhs.y * rhs.m_11 + lhs.z * rhs.m_21 + lhs.w * rhs.m_31;
	float t2 = lhs.x * rhs.m_02 + lhs.y * rhs.m_12 + lhs.z * rhs.m_22 + lhs.w * rhs.m_32;
	float t3 = lhs.x * rhs.m_03 + lhs.y * rhs.m_13 + lhs.z * rhs.m_23 + lhs.w * rhs.m_33;
	return cVector4(t0, t1, t2, t3);
}

eae6320::Math::cMatrix_transformation eae6320::Math::operator*(const eae6320::Math::cMatrix_transformation& lhs, const eae6320::Math::cMatrix_transformation& rhs)
{
	float m_00 = lhs.m_00 * rhs.m_00 + lhs.m_01 * rhs.m_10 + lhs.m_02 * rhs.m_20 + lhs.m_03 * rhs.m_30;
	float m_01 = lhs.m_00 * rhs.m_01 + lhs.m_01 * rhs.m_11 + lhs.m_02 * rhs.m_21 + lhs.m_03 * rhs.m_31;
	float m_02 = lhs.m_00 * rhs.m_02 + lhs.m_01 * rhs.m_12 + lhs.m_02 * rhs.m_22 + lhs.m_03 * rhs.m_32;
	float m_03 = lhs.m_00 * rhs.m_03 + lhs.m_01 * rhs.m_13 + lhs.m_02 * rhs.m_23 + lhs.m_03 * rhs.m_33;
	float m_10 = lhs.m_10 * rhs.m_00 + lhs.m_11 * rhs.m_10 + lhs.m_12 * rhs.m_20 + lhs.m_13 * rhs.m_30;
	float m_11 = lhs.m_10 * rhs.m_01 + lhs.m_11 * rhs.m_11 + lhs.m_12 * rhs.m_21 + lhs.m_13 * rhs.m_31;
	float m_12 = lhs.m_10 * rhs.m_02 + lhs.m_11 * rhs.m_12 + lhs.m_12 * rhs.m_22 + lhs.m_13 * rhs.m_32;
	float m_13 = lhs.m_10 * rhs.m_03 + lhs.m_11 * rhs.m_13 + lhs.m_12 * rhs.m_23 + lhs.m_13 * rhs.m_33;
	float m_20 = lhs.m_20 * rhs.m_00 + lhs.m_21 * rhs.m_10 + lhs.m_22 * rhs.m_20 + lhs.m_23 * rhs.m_30;
	float m_21 = lhs.m_20 * rhs.m_01 + lhs.m_21 * rhs.m_11 + lhs.m_22 * rhs.m_21 + lhs.m_23 * rhs.m_31;
	float m_22 = lhs.m_20 * rhs.m_02 + lhs.m_21 * rhs.m_12 + lhs.m_22 * rhs.m_22 + lhs.m_23 * rhs.m_32;
	float m_23 = lhs.m_20 * rhs.m_03 + lhs.m_21 * rhs.m_13 + lhs.m_22 * rhs.m_23 + lhs.m_23 * rhs.m_33;
	float m_30 = lhs.m_30 * rhs.m_00 + lhs.m_31 * rhs.m_10 + lhs.m_32 * rhs.m_20 + lhs.m_33 * rhs.m_30;
	float m_31 = lhs.m_30 * rhs.m_01 + lhs.m_31 * rhs.m_11 + lhs.m_32 * rhs.m_21 + lhs.m_33 * rhs.m_31;
	float m_32 = lhs.m_30 * rhs.m_02 + lhs.m_31 * rhs.m_12 + lhs.m_32 * rhs.m_22 + lhs.m_33 * rhs.m_32;
	float m_33 = lhs.m_30 * rhs.m_03 + lhs.m_31 * rhs.m_13 + lhs.m_32 * rhs.m_23 + lhs.m_33 * rhs.m_33;
	return cMatrix_transformation(
		m_00, m_10, m_20, m_30,
		m_01, m_11, m_21, m_31,
		m_02, m_12, m_22, m_32,
		m_03, m_13, m_23, m_33);
}

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::Invert(const cMatrix_transformation& i_matrix)
{
	cMatrix_transformation result = i_matrix;
	cMatrix_transformation temp;
	temp.m_00 = temp.m_11 = temp.m_22 = temp.m_33 = 1.0f;

	const int SIZE = 4;

	const float* fromValues = &(i_matrix.m_00);

	float* resultValues = &(result.m_00);

	float* tempValues = &(temp.m_00);

	int indexs[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		indexs[i] = -1;
	}
	for (int i = 0; i < SIZE; i++)
	{
		float t_absMax = 0;
		int t_index = -1;
		//override i with pivot
		for (int j = 0; j < SIZE; j++)
		{
			float t_abs = abs(resultValues[i * SIZE + j]);
			if (t_abs > t_absMax && indexs[j] == -1)
			{
				t_absMax = t_abs;
				t_index = j;
			}
		}
		//because we are using transform matrix which always has inverted matrix
		assert(t_index != -1);
		indexs[t_index] = i;
		float factor = resultValues[i * SIZE + t_index];
		for (int j = 0; j < SIZE; j++)
		{
			resultValues[j * SIZE + t_index] /= factor;
			tempValues[j * SIZE + t_index] /= factor;
		}
		for (int j = 0; j < SIZE; j++)
		{
			if (t_index == j) continue;
			float coefficient = resultValues[i * SIZE + j];
			for (int k = 0; k < SIZE; k++)
			{
				tempValues[k * SIZE + j] -= coefficient * tempValues[k * SIZE + t_index];
				resultValues[k * SIZE + j] -= coefficient * resultValues[k * SIZE + t_index];
			}
		}
	}
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			resultValues[j * SIZE + indexs[i]] = tempValues[j * SIZE + i];
		}
	}

	return result;
}