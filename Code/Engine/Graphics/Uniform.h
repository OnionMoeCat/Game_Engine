/*
This file contains the function declarations for effect
*/

#ifndef EAE6320_UNIFORM_H
#define EAE6320_UNIFORM_H

// Header Files
//=============

#include "Includes.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct UniformVector
		{
			tUniformHandle uniformHandle = 0;
			float values[4];
			ShaderTypes::eShaderType shaderType;
			uint8_t valueCountToSet;
		};

		struct UniformMatrix
		{
			tUniformHandle uniformHandle = 0;
			float values[16];
			ShaderTypes::eShaderType shaderType;
			uint8_t rowsToSet;
		};
	}
}

#endif	// EAE6320_MESH_H
