/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_EFFECTHELPER_H
#define EAE6320_EFFECTHELPER_H

// Header Files
//=============
#include "Context.h"
#include "Effect.h"
#include "Includes.h"
#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		enum RenderStates : uint8_t
		{
			ALPHA = 1 << 0,
			DEPTHTEST = 1 << 1,
			DEPTHWRITE = 1 << 2
		};

		struct EffectHelper
		{
		private:
			bool static LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const Context& i_context);
			bool static CleanUp(Effect& i_effect, const Context& i_context);
		public:
			bool static LoadEffectFromFile(Effect& i_effect, const char* const i_effectPath);
			bool static CleanUp(Effect& i_effect);
			bool static Bind(Effect& i_effect, const Context& i_context);
			bool static GetUniformHandler(const Effect& i_effect, const char* i_name, ShaderTypes::eShaderType i_shaderType, tUniformHandle* i_uniformHandle);
			bool static SetUniform(Effect& i_effect, const float* i_values, const size_t i_valueLength, const tUniformHandle i_uniformHandle, ShaderTypes::eShaderType i_shaderType, const Context& i_context);
			bool static SetMatrixUniform(Effect& i_effect, const float* i_values, const tUniformHandle i_uniformHandle, ShaderTypes::eShaderType i_shaderType, const Context& i_context);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
