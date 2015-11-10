// Header Files
//=============

#include "RenderableHelper.h"

#include "../UserOutput/UserOutput.h"
#include "../Graphics/EffectHelper.h"
#include "../Graphics/MeshHelper.h"

#include <sstream>

// Interface
//==========

bool eae6320::Graphics::RenderableHelper::LoadRenderableFromFile(Renderable& i_renderable, const char* const i_effectPath, const char* i_meshPath)
{
	if (i_renderable.m_effect == NULL)
	{
		i_renderable.m_effect = new eae6320::Graphics::Effect();
	}
	if (i_renderable.m_effect)
	{
		if (!eae6320::Graphics::EffectHelper::LoadEffectFromFile(*i_renderable.m_effect, i_effectPath))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to load effect from effect file: " << i_effectPath;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		eae6320::UserOutput::Print("Fail to initialize effect");
		return false;
	}
	if (i_renderable.m_mesh == NULL)
	{
		i_renderable.m_mesh = new eae6320::Graphics::Mesh();
	}
	if (i_renderable.m_mesh)
	{
		if (!eae6320::Graphics::MeshHelper::LoadMeshFromFile(*i_renderable.m_mesh, i_meshPath))
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

bool eae6320::Graphics::RenderableHelper::CreateLocalToWorldTransform(Renderable& i_renderable, const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position)
{
	if (i_renderable.m_effect == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("renderable.m_effect not null expected");
		return false;
	}
	eae6320::Graphics::EffectHelper::CreateLocalToWorldTransform(*i_renderable.m_effect, i_rotation, i_position);
	return true;
}

bool eae6320::Graphics::RenderableHelper::CreateWorldToViewTransform(Renderable& i_renderable, const eae6320::Math::cQuaternion& i_cameraRotation, const eae6320::Math::cVector& i_cameraPosition)
{
	if (i_renderable.m_effect == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("renderable.m_effect not null expected");
		return false;
	}
	eae6320::Graphics::EffectHelper::CreateWorldToViewTransform(*i_renderable.m_effect, i_cameraRotation, i_cameraPosition);
	return true;
}

bool eae6320::Graphics::RenderableHelper::CreateViewToScreenTransform(Renderable& i_renderable, const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ)
{
	if (i_renderable.m_effect == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("renderable.m_effect not null expected");
		return false;
	}
	eae6320::Graphics::EffectHelper::CreateViewToScreenTransform(*i_renderable.m_effect, i_fov, i_aspect, i_nearZ, i_farZ);
	return true;
}