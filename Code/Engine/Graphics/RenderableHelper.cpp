// Header Files
//=============

#include "RenderableHelper.h"

#include "../UserOutput/UserOutput.h"
#include "../Graphics/MaterialHelper.h"
#include "../Graphics/MeshHelper.h"

#include <sstream>

// Interface
//==========

bool eae6320::Graphics::RenderableHelper::LoadRenderableFromFile(Renderable& i_renderable, const char* const i_materialPath, const char* i_meshPath)
{
	if (i_renderable.m_material == NULL)
	{
		i_renderable.m_material = new eae6320::Graphics::Material();
	}
	if (i_renderable.m_material)
	{
		if (!eae6320::Graphics::MaterialHelper::LoadMaterialFromFile(*i_renderable.m_material, i_materialPath))
		{
			std::stringstream errorMessage;
			errorMessage << "Fail to load effect from effect file: " << i_materialPath;
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
	if (i_entity.m_material)
	{
		if (!eae6320::Graphics::MaterialHelper::CleanUp(*i_entity.m_material))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up effect");
			delete i_entity.m_material;
			return false;
		}
		delete i_entity.m_material;
		i_entity.m_material = NULL;
	}

	if (i_entity.m_mesh)
	{
		if (!eae6320::Graphics::MeshHelper::CleanUp(*i_entity.m_mesh))
		{
			std::stringstream errorMessage;
			eae6320::UserOutput::Print("Fail to clean up mesh");
			delete i_entity.m_mesh;
			return false;
		}
		delete i_entity.m_mesh;
		i_entity.m_mesh = NULL;
	}
	return true;
}