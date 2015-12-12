// Header Files
//=============

#include <sstream>

#include "EntityHelper.h"
#include "TransformHelper.h"
#include "CollidableHelper.h"
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
bool eae6320::Core::EntityHelper::CleanUp(Entity& i_entity)
{
	bool wereThereErrors = false;
	if (i_entity.m_renderable)
	{
		if (!eae6320::Graphics::RenderableHelper::CleanUp(*i_entity.m_renderable))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			delete i_entity.m_renderable;
			wereThereErrors = true;
		}
		delete i_entity.m_renderable;
		i_entity.m_renderable = NULL;
	}
	if (i_entity.m_renderable)
	{
		delete i_entity.m_renderable;
		i_entity.m_renderable = NULL;
	}
	if (i_entity.m_iController)
	{
		delete i_entity.m_iController;
		i_entity.m_iController = NULL;
	}
	return !wereThereErrors;
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
bool eae6320::Core::EntityHelper::SetTransform(Entity& i_entity, const eae6320::Math::cVector& i_position, const eae6320::Math::cQuaternion& i_quaternion, const eae6320::Math::cVector& i_AABB)
{
	if (i_entity.m_transform == NULL)
	{
		i_entity.m_transform = new eae6320::Core::Transform();
	}
	if (i_entity.m_transform)
	{
		TransformHelper::SetPosition(*i_entity.m_transform, *i_entity.m_renderable->m_material->m_effect, i_position);
		TransformHelper::SetRotation(*i_entity.m_transform, *i_entity.m_renderable->m_material->m_effect, i_quaternion);
		TransformHelper::SetAABB(*i_entity.m_transform, i_AABB);
	}
	else
	{
		eae6320::UserOutput::Print("Fail to initialize transform");
		return false;
	}
	return true;
}

bool eae6320::Core::EntityHelper::SetController(Entity& i_entity, IController* i_controller)
{
	i_entity.m_iController = i_controller;
	return true;
}

bool eae6320::Core::EntityHelper::SetCollidable(Entity& i_entity, const float i_mass)
{
	if (i_entity.m_collidable == NULL)
	{
		i_entity.m_collidable = new eae6320::Core::Collidable();
	}
	if (i_entity.m_collidable)
	{
		CollidableHelper::SetMass(*i_entity.m_collidable, i_mass);
	}
	else
	{
		eae6320::UserOutput::Print("Fail to initialize transform");
		return false;
	}
	return true;
}

bool eae6320::Core::EntityHelper::SetName(Entity& i_entity, const char* const i_name)
{
	if (i_entity.m_name != NULL)
	{
		free(i_entity.m_name);
		i_entity.m_name = NULL;
	}
	i_entity.m_name = _strdup(i_name);
	return (i_entity.m_name != NULL);
}

bool eae6320::Core::EntityHelper::SetAlive(Entity& i_entity, const bool i_alive)
{
	i_entity.m_isAlive = i_alive;
	return true;
}