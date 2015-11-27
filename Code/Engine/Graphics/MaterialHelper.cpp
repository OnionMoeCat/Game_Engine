// Header Files
//=============

#include "MaterialHelper.h"

#include <sstream>

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
			i_material.m_uniformCount = *reinterpret_cast<uint32_t*>(tempPTR);
			i_material.m_uniform = new Uniform[i_material.m_uniformCount];
			if (i_material.m_uniform == NULL)
			{
				wereThereErrors = true;
				goto OnExit;
			}

			tempPTR += sizeof(uint32_t);

			memcpy(i_material.m_uniform, tempPTR, sizeof(Uniform) * i_material.m_uniformCount);

			tempPTR += sizeof(Uniform) * i_material.m_uniformCount;

			for (uint32_t i = 0; i < i_material.m_uniformCount; i++)
			{
				uint8_t strLength = *reinterpret_cast<uint8_t*>(tempPTR);
				tempPTR += sizeof(uint8_t);
				const char* uniformName = reinterpret_cast<char*>(tempPTR);
				//get handler
				if (!EffectHelper::GetUniformHandler(*i_material.m_effect, uniformName, i_material.m_uniform[i].shaderType, &i_material.m_uniform[i].uniformHandle))
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

				if (!TextureHelper::LoadTextureFromFile(i_material.m_texture[i], texturePath))
				{
					wereThereErrors = true;
					goto OnExit;
				}

				if (!GetSampleID(i_material))
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
	if (i_material.m_effect != NULL)
	{
		if (!eae6320::Graphics::EffectHelper::CleanUp(*i_material.m_effect))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			delete i_material.m_effect;
			return false;
		}
		delete i_material.m_effect;
		i_material.m_effect = NULL;
	}

	if (i_material.m_uniform != NULL)
	{
		delete[] i_material.m_uniform;
		i_material.m_uniform = NULL;
	}

	i_material.m_uniformCount = 0;

	return true;
}

bool eae6320::Graphics::MaterialHelper::SetMaterialUniforms(Material& i_material, const Context& i_context)
{
	if (i_material.m_uniform == NULL)
	{
		return false;
	}

	for (size_t i = 0; i < i_material.m_uniformCount; i++)
	{
		if (!EffectHelper::SetUniform(*i_material.m_effect, i_material.m_uniform[i].values, i_material.m_uniform[i].valueCountToSet, i_material.m_uniform[i].uniformHandle, i_material.m_uniform[i].shaderType, i_context))
		{
			return false;
		}
	}

	return true;
}