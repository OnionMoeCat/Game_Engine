// Header Files
//=============

#include "RenderableHelper.h"

#include "../UserOutput/UserOutput.h"
#include "../Graphics/EffectHelper.h"
#include "../Graphics/MeshHelper.h"

#include <sstream>

// Interface
//==========

bool eae6320::Graphics::RenderableHelper::LoadEntityFromFile(Renderable& i_entity, const char* const i_vertexPath, const char* const i_fragmentPath, const char* i_meshPath)
{
	if (i_entity.m_effect == NULL)
	{
		i_entity.m_effect = new eae6320::Graphics::Effect();
	}
	if (i_entity.m_effect)
	{
		if (!eae6320::Graphics::EffectHelper::LoadEffectFromFile(*i_entity.m_effect, i_vertexPath, i_fragmentPath))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to load effect from vertex shader file: " << i_vertexPath << " and fragment shader file: " << i_fragmentPath;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		eae6320::UserOutput::Print("Fail to initialize effect");
		return false;
	}
	if (i_entity.m_mesh == NULL)
	{
		i_entity.m_mesh = new eae6320::Graphics::Mesh();
	}
	if (i_entity.m_mesh)
	{
		if (!eae6320::Graphics::MeshHelper::LoadMeshFromFile(*i_entity.m_mesh, i_meshPath))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to load effect from mesh file: " << i_meshPath;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		eae6320::UserOutput::Print("Fail to initialize mesh");
		return false;
	}
	return true;
}

void eae6320::Graphics::RenderableHelper::OffsetPosition(Renderable& i_entity, const eae6320::Math::cVector& i_offset_position)
{
	i_entity.cVector += i_offset_position;
}

bool eae6320::Graphics::RenderableHelper::CleanUp(Renderable& i_entity)
{
	if (i_entity.m_effect)
	{
		if (!eae6320::Graphics::EffectHelper::CleanUp(*i_entity.m_effect))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			return false;
		}
		i_entity.m_effect = NULL;
	}

	if (i_entity.m_mesh)
	{
		if (!eae6320::Graphics::MeshHelper::CleanUp(*i_entity.m_mesh))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up mesh");
			return false;
		}
		i_entity.m_mesh = NULL;
	}
	return true;
}