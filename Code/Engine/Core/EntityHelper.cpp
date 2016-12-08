// Header Files
//=============

#include <sstream>

#include "EntityHelper.h"
#include "CameraHelper.h"
#include "../UserOutput/UserOutput.h"
#include "../Graphics/RenderableHelper.h"
#include "../Graphics/RenderableManager.h"
#include "../Graphics/MaterialHelper.h"

// Interface
//==========

eae6320::Math::cMatrix_transformation eae6320::Core::EntityHelper::CreateLocalToWorldTransform(const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position, const eae6320::Math::cVector& i_scale)
{
	return std::move(eae6320::Math::cMatrix_transformation(i_rotation, i_position, i_scale));
}
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
bool eae6320::Core::EntityHelper::OffsetTransform(Entity& i_entity, const eae6320::Math::cVector& i_offset_position, const eae6320::Math::cQuaternion& i_offset_rotation)
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
	i_entity.m_rotation = i_entity.m_rotation * i_offset_rotation;
	
	{
		eae6320::Math::cMatrix_transformation&& temp = CreateLocalToWorldTransform(i_entity.m_rotation, i_entity.m_position, i_entity.m_scale);
		eae6320::Graphics::MaterialHelper::UpdateMaterialUniformMatrix(*i_entity.m_renderable->m_material, "g_transform_localToWorld", temp, eae6320::Graphics::ShaderTypes::Vertex);
	}

	{
		eae6320::Math::cMatrix_transformation&& temp = eae6320::Math::cMatrix_transformation::CreateNormalMatrix(i_entity.m_rotation, i_entity.m_position, i_entity.m_scale);
		eae6320::Graphics::MaterialHelper::UpdateMaterialUniformMatrix(*i_entity.m_renderable->m_material, "g_transform_normalMatrix", temp, eae6320::Graphics::ShaderTypes::Vertex);
	}

	return true;
}
void eae6320::Core::EntityHelper::Scale(Entity& i_entity, const eae6320::Math::cVector& i_scale)
{
	i_entity.m_scale = i_scale;
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

	{
		eae6320::Math::cMatrix_transformation&& temp = CameraHelper::CreateWorldToViewTransform(i_camera.m_rotation, i_camera.m_position);
		eae6320::Graphics::MaterialHelper::UpdateMaterialUniformMatrix(*i_entity.m_renderable->m_material, "g_transform_worldToView", temp, eae6320::Graphics::ShaderTypes::Vertex);
	}

	{
		eae6320::Math::cMatrix_transformation&& temp = CameraHelper::CreateViewToScreenTransform(i_camera.m_fov, i_camera.m_aspect, i_camera.m_nearZ, i_camera.m_farZ);
		eae6320::Graphics::MaterialHelper::UpdateMaterialUniformMatrix(*i_entity.m_renderable->m_material, "g_transform_viewToScreen", temp, eae6320::Graphics::ShaderTypes::Vertex);
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

bool eae6320::Core::EntityHelper::AddLights(Entity& i_entity, eae6320::Math::cVector& i_light_position)
{
	return eae6320::Graphics::MaterialHelper::UpdateMaterialUniformVector(*i_entity.m_renderable->m_material, "g_light_position_world", reinterpret_cast<float*>(&i_light_position), static_cast<uint8_t>(sizeof(eae6320::Math::cVector)/sizeof(float)), eae6320::Graphics::ShaderTypes::Fragment);
}

bool eae6320::Core::EntityHelper::EyePosition(Entity& i_entity, eae6320::Math::cVector& i_eye_position)
{
	return eae6320::Graphics::MaterialHelper::UpdateMaterialUniformVector(*i_entity.m_renderable->m_material, "g_eye_position_world", reinterpret_cast<float*>(&i_eye_position), static_cast<uint8_t>(sizeof(eae6320::Math::cVector) / sizeof(float)), eae6320::Graphics::ShaderTypes::Fragment);
}