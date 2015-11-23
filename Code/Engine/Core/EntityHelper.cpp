// Header Files
//=============

#include <sstream>

#include "EntityHelper.h"
#include "../UserOutput/UserOutput.h"
#include "../Graphics/RenderableHelper.h"
#include "../Graphics/RenderableManager.h"
#include "../Graphics/EffectHelper.h"

// Interface
//==========

bool eae6320::Core::EntityHelper::LoadEntityFromFile(Entity& i_entity, const char* const i_materialPath, const char* i_meshPath)
{
	if (i_entity.m_renderable == NULL)
	{
		i_entity.m_renderable = new eae6320::Graphics::Renderable();
	}
	if (i_entity.m_renderable)
	{
		if (!eae6320::Graphics::RenderableHelper::LoadRenderableFromFile(*i_entity.m_renderable, i_materialPath, i_meshPath))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to load renderable. Material path: " << i_materialPath << " Mesh path: " << i_meshPath;
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	}
	else
	{
		eae6320::UserOutput::Print("Fail to initialize renderable");
		return false;
	}
	return true;
}
bool eae6320::Core::EntityHelper::OffsetPosition(Entity& i_entity, const eae6320::Math::cVector& i_offset_position)
{
	if (i_entity.m_renderable == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable not null expected");
		return false;
	}
	if (i_entity.m_renderable->m_material == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable->m_material not null expected");
		return false;
	}
	if (i_entity.m_renderable->m_material->m_effect == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable->m_material->m_effect not null expected");
		return false;
	}
	i_entity.m_position += i_offset_position;
	eae6320::Graphics::EffectHelper::CreateLocalToWorldTransform(*i_entity.m_renderable->m_material->m_effect, i_entity.m_rotation, i_entity.m_position);
	return true;
}
bool eae6320::Core::EntityHelper::CleanUp(Entity& i_entity)
{
	if (i_entity.m_renderable)
	{
		if (!eae6320::Graphics::RenderableHelper::CleanUp(*i_entity.m_renderable))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			delete i_entity.m_renderable;
			return false;
		}
		delete i_entity.m_renderable;
		i_entity.m_renderable = NULL;
	}
	return true;
}
bool eae6320::Core::EntityHelper::ToCameraScreen(Entity& i_entity, const Camera& i_camera)
{
	if (i_entity.m_renderable == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable not null expected");
		return false;
	}
	if (i_entity.m_renderable->m_material == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable->m_material not null expected");
		return false;
	}
	if (i_entity.m_renderable->m_material->m_effect == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable->m_material->m_effect not null expected");
		return false;
	}
	eae6320::Graphics::EffectHelper::CreateWorldToViewTransform(*i_entity.m_renderable->m_material->m_effect, i_camera.m_rotation, i_camera.m_position);
	eae6320::Graphics::EffectHelper::CreateViewToScreenTransform(*i_entity.m_renderable->m_material->m_effect, i_camera.m_fov, i_camera.m_aspect, i_camera.m_nearZ, i_camera.m_farZ);
	return true;
}
bool eae6320::Core::EntityHelper::Submit(Entity& i_entity)
{
	if (i_entity.m_renderable == NULL)
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("entity.m_renderable not null expected");
		return false;
	}
	eae6320::Graphics::RenderableManager::Get().Submit(*i_entity.m_renderable);
	return true;
}