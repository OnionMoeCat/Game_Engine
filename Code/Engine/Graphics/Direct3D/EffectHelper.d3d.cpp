// Header Files
//=============

#include "../EffectHelper.h"

#include "../../Windows/Includes.h"

#include "../../UserOutput/UserOutput.h"
#include "../../Utils/Utils.h"

// Helper Function Declarations
//=============================

namespace
{
	bool LoadVertexShader(eae6320::Graphics::Effect& i_effect, const char* const i_vertexPath, IDirect3DDevice9* i_device);
	bool LoadFragmentShader(eae6320::Graphics::Effect& i_effect, const char* const i_fragmentPath, IDirect3DDevice9* i_device);
}

bool eae6320::Graphics::EffectHelper::LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const Context& i_context)
{
	if (i_context.device == NULL)
	{
		return false;
	}
	if (!LoadVertexShader(i_effect, i_vertexPath, i_context.device))
	{
		return false;
	}
	if (!LoadFragmentShader(i_effect, i_fragmentPath, i_context.device))
	{
		return false;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::Bind(Effect& i_effect, const Context& i_context)
{
	if (i_context.device == NULL)
	{
		return false;
	}
	HRESULT result = i_context.device->SetVertexShader(i_effect.m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	result = i_context.device->SetPixelShader(i_effect.m_fragmentShader);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::CleanUp(Effect& i_effect, const Context& i_context)
{
	if (i_effect.m_vertexShader)
	{
		i_effect.m_vertexShader->Release();
		i_effect.m_vertexShader = NULL;
	}
	if (i_effect.m_fragmentShader)
	{
		i_effect.m_fragmentShader->Release();
		i_effect.m_fragmentShader = NULL;
	}
	return true;
}
bool eae6320::Graphics::EffectHelper::SetDrawCallUniforms(Effect& i_effect, const eae6320::Math::cVector& i_vector, const Context& i_Context)
{
	if (i_effect.m_vertexShaderConstantTable != NULL)
	{
		D3DXHANDLE handle = i_effect.m_vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");
		if (handle != NULL)
		{
			const unsigned int floatCount = 3;
			const float floatArray[floatCount] = { i_vector.x, i_vector.y, i_vector.z };
			if (i_Context.device == NULL)
			{
				return false;
			}
			HRESULT result = i_effect.m_vertexShaderConstantTable->SetFloatArray(i_Context.device, handle, floatArray, floatCount);
			if (FAILED(result))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

namespace
{
	bool LoadVertexShader(eae6320::Graphics::Effect& i_effect, const char* const i_vertexPath, IDirect3DDevice9* i_device)
	{
		bool wereThereErrors = false;
		// Load binary vertex shader file
		void* temporaryBuffer = NULL;
		{
			if (!eae6320::Utils::ReadDataFromFile(i_vertexPath, &temporaryBuffer))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		// Create the vertex shader object
		{
			if (i_device != NULL)
			{
				HRESULT result = i_device->CreateVertexShader(reinterpret_cast<DWORD*>(temporaryBuffer),
					&i_effect.m_vertexShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
					wereThereErrors = true;
					goto OnExit;
				}

				ID3DXConstantTable* vertexShaderConstantTable = NULL;
				result = D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(temporaryBuffer), &vertexShaderConstantTable);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to load constant table of vertex shader");
					wereThereErrors = true;
					goto OnExit;
				}
				else
				{
					i_effect.m_vertexShaderConstantTable = vertexShaderConstantTable;
				}
			}
			else
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:

		if (temporaryBuffer != NULL)
		{
			free(temporaryBuffer);
		}
		return !wereThereErrors;
	}
	bool LoadFragmentShader(eae6320::Graphics::Effect& i_effect, const char* const i_fragmentPath, IDirect3DDevice9* i_device)
	{
		bool wereThereErrors = false;
		// Load binary fragment shader file
		void* temporaryBuffer = NULL;
		{
			if (!eae6320::Utils::ReadDataFromFile(i_fragmentPath, &temporaryBuffer))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		// Create the fragment shader object
		{
			if (i_device != NULL)
			{
				HRESULT result = i_device->CreatePixelShader(reinterpret_cast<DWORD*>(temporaryBuffer),
					&i_effect.m_fragmentShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
					wereThereErrors = true;
				}
			}
			else
			{
				wereThereErrors = true;
			}
		}

	OnExit:

		if (temporaryBuffer != NULL)
		{
			free(temporaryBuffer);
		}
		return !wereThereErrors;
	}
}