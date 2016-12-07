/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_MATERIALHELPER_H
#define EAE6320_MATERIALHELPER_H

// Header Files
//=============
#include "Material.h"

#include "Context.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct MaterialHelper
		{
		public:
			bool static LoadMaterialFromFile(Material& i_material, const char* const i_path);
			bool static SetMaterialUniforms(Material& i_material, const Context& i_context);
			bool static SetMaterialTextures(Material& i_material, const Context& i_context);
			bool static UpdateMaterialUniformMatrix(Material& i_material, const std::string& i_name,  eae6320::Math::cMatrix_transformation& i_matrix, const ShaderTypes::eShaderType i_shader_type);
			bool static UpdateMaterialUniformVector(Material& i_material, const std::string& i_name, float* i_vector, const uint8_t i_count, const ShaderTypes::eShaderType i_shader_type);
			bool static CleanUp(Material& i_material);		
		private:
			bool static UpdateMaterialUniformMatrix(Material& i_material, const std::string& i_name, eae6320::Math::cMatrix_transformation& i_matrix, const ShaderTypes::eShaderType i_shader_type, const Context& i_context);
			bool static UpdateMaterialUniformVector(Material& i_material, const std::string& i_name, float* i_vector, const uint8_t i_count, const ShaderTypes::eShaderType i_shader_type, const Context& i_context);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
