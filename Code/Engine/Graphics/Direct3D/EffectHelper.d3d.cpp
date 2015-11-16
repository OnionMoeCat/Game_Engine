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
	bool SetMatrixUniform(eae6320::Graphics::Effect& i_effect, const char* const i_variable, eae6320::Math::cMatrix_transformation& i_matrix, IDirect3DDevice9* i_device);
	namespace RenderStates
	{
		const uint8_t ALPHA = 1 << 0;
		const uint8_t DEPTHTEST = 1 << 1;
		const uint8_t DEPTHWRITE = 1 << 2;
	}
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

	uint8_t renderStates = i_effect.m_renderStates;
	
	HRESULT result;
	if (renderStates & RenderStates::ALPHA)
	{
		result = i_context.device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		if (FAILED(result))
		{
			return false;
		}
		result = i_context.device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		if (FAILED(result))
		{
			return false;
		}
		result = i_context.device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		if (FAILED(result))
		{
			return false;
		}
	}
	else
	{
		result = i_context.device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		if (FAILED(result))
		{
			return false;
		}
	}

	if (renderStates & RenderStates::DEPTHTEST)
	{
		result = i_context.device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		if (FAILED(result))
		{
			return false;
		}
		result = i_context.device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		if (FAILED(result))
		{
			return false;
		}
	}
	else
	{
		result = i_context.device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		if (FAILED(result))
		{
			return false;
		}
	}

	if (renderStates & RenderStates::DEPTHWRITE)
	{
		result = i_context.device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		if (FAILED(result))
		{
			return false;
		}
	}
	else
	{
		result = i_context.device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		if (FAILED(result))
		{
			return false;
		}
	}

	result = i_context.device->SetVertexShader(i_effect.m_vertexShader);
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
bool eae6320::Graphics::EffectHelper::SetDrawCallUniforms(Effect& i_effect, const Context& i_Context)
{
	if (i_Context.device == NULL)
	{
		return false;
	}
	if (!SetMatrixUniform(i_effect, "g_transform_localToWorld", i_effect.m_transform_localToWorld, i_Context.device))
	{
		return false;
	}
	if (!SetMatrixUniform(i_effect, "g_transform_worldToView", i_effect.m_transform_worldToView, i_Context.device))
	{
		return false;
	}
	if (!SetMatrixUniform(i_effect, "g_transform_viewToScreen", i_effect.m_transform_viewToScreen, i_Context.device))
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
	bool SetMatrixUniform(eae6320::Graphics::Effect& i_effect, const char* const i_variable, eae6320::Math::cMatrix_transformation& i_matrix, IDirect3DDevice9* i_device)
	{
		if (i_effect.m_vertexShaderConstantTable != NULL)
		{
			D3DXHANDLE handle = i_effect.m_vertexShaderConstantTable->GetConstantByName(NULL, i_variable);
			if (handle != NULL)
			{

				HRESULT result = i_effect.m_vertexShaderConstantTable->SetMatrixTranspose(i_device, handle,
					reinterpret_cast<const D3DXMATRIX*>(&i_matrix));
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
}