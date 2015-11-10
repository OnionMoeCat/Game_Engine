// Header Files
//=============

#include <sstream>

#include "EntityHelper.h"
#include "../UserOutput/UserOutput.h"
#include "../Graphics/RenderableHelper.h"
#include "../Graphics/RenderableManager.h"

// Interface
//==========

bool eae6320::Core::EntityHelper::LoadEntityFromFile(Entity& i_entity, const char* const i_effectPath, const char* i_meshPath)
{
	if (i_entity.m_renderable == NULL)
	{
		i_entity.m_renderable = new eae6320::Graphics::Renderable();
	}
	if (i_entity.m_renderable)
	{
		if (!eae6320::Graphics::RenderableHelper::LoadRenderableFromFile(*i_entity.m_renderable, i_effectPath, i_meshPath))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to load renderable. Effect path: " << i_effectPath << " Mesh path: " << i_meshPath;
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
		return false;
	}
	i_entity.m_position += i_offset_position;
	return eae6320::Graphics::RenderableHelper::CreateLocalToWorldTransform(*i_entity.m_renderable, i_entity.m_rotation, i_entity.m_position);
}
bool eae6320::Core::EntityHelper::CleanUp(Entity& i_entity)
{
	if (i_entity.m_renderable)
	{
		if (!eae6320::Graphics::RenderableHelper::CleanUp(*i_entity.m_renderable))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			return false;
		}
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
	if (!eae6320::Graphics::RenderableHelper::CreateWorldToViewTransform(*i_entity.m_renderable, i_camera.m_rotation, i_camera.m_position))
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("Fail to create world to view transform");
		return false;
	}
	if (!eae6320::Graphics::RenderableHelper::CreateViewToScreenTransform(*i_entity.m_renderable, i_camera.m_fov, i_camera.m_aspect, i_camera.m_nearZ, i_camera.m_farZ))
	{
		std::stringstream errorMessage;
		eae6320::UserOutput::Print("Fail to create view to screen transform");
		return false;
	}
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