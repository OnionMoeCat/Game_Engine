// Header Files
//=============

#include "RenderableManager.h"
#include "EffectHelper.h"

#include <cassert>

// Interface
//==========

eae6320::Graphics::RenderableManager eae6320::Graphics::RenderableManager::m_Instance;

eae6320::Graphics::RenderableManager& eae6320::Graphics::RenderableManager::Get()
{
	return m_Instance;
}

void eae6320::Graphics::RenderableManager::CleanUp()
{
	m_transparent_list.clear();
	m_opaque_list.clear();
}

void eae6320::Graphics::RenderableManager::Submit(const Renderable& i_renderable)
{
	if (i_renderable.m_material->m_effect->m_renderStates & RenderStates::ALPHA)
	{
		m_transparent_list.push_back(i_renderable);
	}
	else
	{
		m_opaque_list.push_back(i_renderable);
	}
}

size_t eae6320::Graphics::RenderableManager::GetSize()
{
	return m_transparent_list.size() + m_opaque_list.size();
}

eae6320::Graphics::Renderable* eae6320::Graphics::RenderableManager::GetRenderableAtIndex(size_t i_index)
{
	if (i_index < m_opaque_list.size())
	{
		return &m_opaque_list[i_index];
	}

	i_index -= m_opaque_list.size();
	if (i_index < m_transparent_list.size())
	{
		return &m_transparent_list[i_index];
	}

	assert(false);
	return NULL;
}