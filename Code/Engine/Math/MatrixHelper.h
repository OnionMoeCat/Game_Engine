/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_MATRIXHELPER_H
#define EAE6320_MATRIXHELPER_H

#include "cMatrix_transformation.h"

// Interface
//==========

namespace eae6320
{
	namespace Math
	{
		namespace MatrixHelper
		{
			cMatrix_transformation CreateScale(const cQuaternion& i_vector3);
			cMatrix_transformation CreateIdentity();
		}
	}
}

#endif	// EAE6320_GRAPHICS_H
