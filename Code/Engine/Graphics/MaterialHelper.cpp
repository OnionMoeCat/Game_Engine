// Header Files
//=============

#include "MaterialHelper.h"

#include <sstream>
#include <cassert>

#include "EffectHelper.h"
#include "TextureHelper.h"
#include "../UserOutput/UserOutput.h"
#include "../Utils/Utils.h"

// Interface
//==========

bool eae6320::Graphics::MaterialHelper::LoadMaterialFromFile(Material& i_material, const char* const i_materialPath)
{
	bool wereThereErrors = false;

	void* temporaryBuffer = NULL;
	{
		if (eae6320::Utils::ReadDataFromFile(i_materialPath, &temporaryBuffer))
		{
			char* tempPTR = reinterpret_cast<char*>(temporaryBuffer);
			uint8_t strLength = *reinterpret_cast<uint8_t*>(tempPTR);
			tempPTR += sizeof(uint8_t);
			const char* effectPath = reinterpret_cast<char*>(tempPTR);

			i_material.m_effect = new eae6320::Graphics::Effect();
			if (i_material.m_effect == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}

			if (!EffectHelper::LoadEffectFromFile(*i_material.m_effect, effectPath))
			{
				wereThereErrors = true;
				goto OnExit;
			}

			tempPTR += strLength;
			i_material.m_uniformVectorCount = *reinterpret_cast<uint32_t*>(tempPTR);
			i_material.m_uniformVector = new UniformVector[i_material.m_uniformVectorCount];
			if (i_material.m_uniformVector == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}

			tempPTR += sizeof(uint32_t);

			memcpy(i_material.m_uniformVector, tempPTR, sizeof(UniformVector) * i_material.m_uniformVectorCount);

			tempPTR += sizeof(UniformVector) * i_material.m_uniformVectorCount;

			for (uint32_t i = 0; i < i_material.m_uniformVectorCount; i++)
			{
				uint8_t strLength = *reinterpret_cast<uint8_t*>(tempPTR);
				tempPTR += sizeof(uint8_t);
				const char* uniformName = reinterpret_cast<char*>(tempPTR);
				//get handler
				if (!EffectHelper::GetUniformHandler(*i_material.m_effect, uniformName, i_material.m_uniformVector[i].shaderType, &i_material.m_uniformVector[i].uniformHandle))
				{
					wereThereErrors = true;
					goto OnExit;
				}			
				tempPTR += strLength;
			}

			i_material.m_textureCount = *reinterpret_cast<uint32_t*>(tempPTR);
			i_material.m_texture = new Texture[i_material.m_textureCount];
			if (i_material.m_texture == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}

			tempPTR += sizeof(uint32_t);

			for (uint32_t i = 0; i < i_material.m_textureCount; i++)
			{
				uint8_t strLength = *reinterpret_cast<uint8_t*>(tempPTR);
				tempPTR += sizeof(uint8_t);
				const char* textureName = reinterpret_cast<char*>(tempPTR);
				tempPTR += strLength;
				strLength = *reinterpret_cast<uint8_t*>(tempPTR);
				tempPTR += sizeof(uint8_t);
				const char* texturePath = reinterpret_cast<char*>(tempPTR);
				tempPTR += strLength;
				ShaderTypes::eShaderType shaderType = *reinterpret_cast<ShaderTypes::eShaderType*>(tempPTR);
				tempPTR += sizeof(ShaderTypes::eShaderType);

				if (!TextureHelper::LoadTextureFromFile(i_material.m_texture[i], texturePath))
				{
					wereThereErrors = true;
					goto OnExit;
				}

				if (!TextureHelper::GetSampleID(i_material.m_texture[i], *(i_material.m_effect), textureName, shaderType))
				{
					wereThereErrors = true;
					goto OnExit;
				}
				
			}

		}
		else
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}


OnExit:

	if (temporaryBuffer)
	{
		free(temporaryBuffer);
	}
	temporaryBuffer = NULL;

	return !wereThereErrors;
}

bool eae6320::Graphics::MaterialHelper::CleanUp(Material& i_material)
{
	bool wereThereErrors = false;
	if (i_material.m_effect != NULL)
	{
		if (!eae6320::Graphics::EffectHelper::CleanUp(*i_material.m_effect))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			delete i_material.m_effect;
			wereThereErrors = true;
		}
		delete i_material.m_effect;
		i_material.m_effect = NULL;
	}

	if (i_material.m_uniformVector != NULL)
	{
		delete[] i_material.m_uniformVector;
		i_material.m_uniformVector = NULL;
	}

	i_material.m_uniformVectorCount = 0;

	if (i_material.m_uniformVectorAdded != NULL)
	{
		free(i_material.m_uniformVectorAdded);
		i_material.m_uniformVectorAdded = NULL;
		i_material.m_uniformVectorAddedCapacity = 0;
		i_material.m_uniformVectorAddedCount = 0;
	}

	if (i_material.m_uniformMatrixAdded != NULL)
	{
		free(i_material.m_uniformMatrixAdded);
		i_material.m_uniformMatrixAdded = NULL;
		i_material.m_uniformMatrixAddedCapacity = 0;
		i_material.m_uniformMatrixAddedCount = 0;
	}

	if (i_material.m_texture != NULL)
	{
		for (size_t i = 0; i < i_material.m_textureCount; i++)
		{
			if (!eae6320::Graphics::TextureHelper::CleanUp(i_material.m_texture[i]))
			{
				std::stringstream errorMessage;
				eae6320::UserOutput::Print("Fail to clean up texture");
				delete i_material.m_effect;
				wereThereErrors = true;
			}
		}
		delete[] i_material.m_texture;
		i_material.m_texture = NULL;
	}

	i_material.m_textureCount = 0;

	return !wereThereErrors;
}

bool eae6320::Graphics::MaterialHelper::SetMaterialUniforms(Material& i_material, const Context& i_context)
{
	if (i_material.m_uniformVector == NULL)
	{
		return false;
	}

	for (size_t i = 0; i < i_material.m_uniformVectorCount; i++)
	{
		if (!EffectHelper::SetUniform(*i_material.m_effect, i_material.m_uniformVector[i].values, i_material.m_uniformVector[i].valueCountToSet, i_material.m_uniformVector[i].uniformHandle, i_material.m_uniformVector[i].shaderType, i_context))
		{
			return false;
		}
	}

	for (size_t i = 0; i < i_material.m_uniformVectorAddedCount; i++)
	{
		if (!EffectHelper::SetUniform(*i_material.m_effect, i_material.m_uniformVectorAdded[i].values, i_material.m_uniformVectorAdded[i].valueCountToSet, i_material.m_uniformVectorAdded[i].uniformHandle, i_material.m_uniformVectorAdded[i].shaderType, i_context))
		{
			return false;
		}
	}

	for (size_t i = 0; i < i_material.m_uniformMatrixAddedCount; i++)
	{
		if (!EffectHelper::SetMatrixUniform(*i_material.m_effect, i_material.m_uniformMatrixAdded[i].values, i_material.m_uniformMatrixAdded[i].uniformHandle, i_material.m_uniformMatrixAdded[i].shaderType, i_context))
		{
			return false;
		}
	}

	return true;
}

bool eae6320::Graphics::MaterialHelper::SetMaterialTextures(Material& i_material, const Context& i_context)
{
	for (size_t i = 0; i < i_material.m_textureCount; i++)
	{
		if (!TextureHelper::SetTexture(i_material.m_texture[i], 
#ifdef EAE6320_GRAPHICS_ISTEXTUREINDEXREQUIREDTOSETTEXTURE
			i,
#endif			
			i_context))
		{
			return false;
		}
	}
	return true;
}

bool eae6320::Graphics::MaterialHelper::UpdateMaterialUniformMatrix(Material& i_material, const std::string& i_name, eae6320::Math::cMatrix_transformation& i_matrix, const ShaderTypes::eShaderType i_shader_type)
{
	return UpdateMaterialUniformMatrix(i_material, i_name, i_matrix, i_shader_type, Context::Get());
}

bool eae6320::Graphics::MaterialHelper::UpdateMaterialUniformMatrix(Material& i_material, const std::string& i_name, eae6320::Math::cMatrix_transformation& i_matrix, const ShaderTypes::eShaderType i_shader_type, const Context& i_context)
{
	// Decide if to insert or update
	{
		auto iterator = i_material.m_uniformMatrixMap.find(i_name);
		// if we have inserted before, update and return 
		if (iterator != i_material.m_uniformMatrixMap.end()) {
			uint32_t index = iterator->second;
			memcpy(i_material.m_uniformMatrixAdded[index].values,
				reinterpret_cast<float*>(&i_matrix), sizeof(i_matrix));
			return true;
		}
	}

	// Initialize if empty
	if (!i_material.m_uniformMatrixAdded) {
		const int INTIALIZESIZE = 1;
		UniformMatrix* temp = reinterpret_cast<UniformMatrix*>(malloc(sizeof(UniformMatrix) * INTIALIZESIZE));
		if (!temp) {
			return false;
		}
		i_material.m_uniformMatrixAddedCapacity = INTIALIZESIZE;
		i_material.m_uniformMatrixAdded = temp;
		i_material.m_uniformMatrixAddedCount = 0;
	}

	// Resize if full
	if (i_material.m_uniformMatrixAddedCount == i_material.m_uniformMatrixAddedCapacity) {
		UniformMatrix* temp = reinterpret_cast<UniformMatrix*>(realloc(i_material.m_uniformMatrixAdded, sizeof(UniformMatrix) * i_material.m_uniformMatrixAddedCapacity * 2));
		if (!temp) {
			return false;
		}
		i_material.m_uniformMatrixAdded = temp;
		i_material.m_uniformMatrixAddedCapacity *= 2;
	}
	// Add one
	{
		// Get handler
		int temp = -1;
		if (!EffectHelper::GetUniformHandler(*i_material.m_effect, i_name.c_str(), i_shader_type, &temp))
		{
			return false;
		}
		const int ROWS = 4;
		i_material.m_uniformMatrixAdded[i_material.m_uniformMatrixAddedCount].rowsToSet = ROWS;
		i_material.m_uniformMatrixAdded[i_material.m_uniformMatrixAddedCount].shaderType = i_shader_type;
		i_material.m_uniformMatrixAdded[i_material.m_uniformMatrixAddedCount].uniformHandle = temp;
		memcpy(i_material.m_uniformMatrixAdded[i_material.m_uniformMatrixAddedCount].values,
			reinterpret_cast<float*>(&i_matrix), sizeof(i_matrix));
		i_material.m_uniformMatrixMap[i_name] = i_material.m_uniformMatrixAddedCount;
		i_material.m_uniformMatrixAddedCount++;
	}
	return true;
}

bool eae6320::Graphics::MaterialHelper::UpdateMaterialUniformVector(Material& i_material, const std::string& i_name, float* i_vector, const uint8_t i_count, const ShaderTypes::eShaderType i_shader_type)
{
	return UpdateMaterialUniformVector(i_material, i_name, i_vector, i_count, i_shader_type, Context::Get());
}

bool eae6320::Graphics::MaterialHelper::UpdateMaterialUniformVector(Material& i_material, const std::string& i_name, float* i_vector, const uint8_t i_count, const ShaderTypes::eShaderType i_shader_type, const Context& i_context)
{
	// Decide if to insert or update
	{
		auto iterator = i_material.m_uniformVectorMap.find(i_name);
		// if we have inserted before, update and return 
		if (iterator != i_material.m_uniformVectorMap.end()) {
			uint32_t index = iterator->second;
			i_material.m_uniformVectorAdded[index].valueCountToSet = i_count;
			memcpy(i_material.m_uniformVectorAdded[index].values,
				i_vector, i_count * sizeof(float));
			return true;
		}
	}

	// Initialize if empty
	if (!i_material.m_uniformVectorAdded) {
		const int INTIALIZESIZE = 1;
		UniformVector* temp = reinterpret_cast<UniformVector*>(malloc(sizeof(UniformVector) * INTIALIZESIZE));
		if (!temp) {
			return false;
		}
		i_material.m_uniformVectorAddedCapacity = INTIALIZESIZE;
		i_material.m_uniformVectorAdded = temp;
		i_material.m_uniformVectorAddedCount = 0;
	}
	// Resize if full
	if (i_material.m_uniformVectorAddedCount == i_material.m_uniformVectorAddedCapacity) {
		UniformVector* temp = reinterpret_cast<UniformVector*>(realloc(i_material.m_uniformVectorAdded, sizeof(UniformVector) * i_material.m_uniformVectorAddedCapacity * 2));
		if (!temp) {
			return false;
		}
		i_material.m_uniformVectorAdded = temp;
		i_material.m_uniformVectorAddedCapacity *= 2;
	}
	// Add one
	{
		// Get handler
		int temp = -1;
		if (!EffectHelper::GetUniformHandler(*i_material.m_effect, i_name.c_str(), i_shader_type, &temp))
		{
			return false;
		}
		i_material.m_uniformVectorAdded[i_material.m_uniformVectorAddedCount].valueCountToSet = i_count;
		i_material.m_uniformVectorAdded[i_material.m_uniformVectorAddedCount].shaderType = i_shader_type;
		i_material.m_uniformVectorAdded[i_material.m_uniformVectorAddedCount].uniformHandle = temp;
		memcpy(i_material.m_uniformVectorAdded[i_material.m_uniformVectorAddedCount].values,
			i_vector, i_count * sizeof(float));
		i_material.m_uniformVectorMap[i_name] = i_material.m_uniformVectorAddedCount;
		i_material.m_uniformVectorAddedCount++;
	}
	return true;
}