// Header Files
//=============

#include "RenderableManager.h"

// Interface
//==========

eae6320::Graphics::RenderableManager eae6320::Graphics::RenderableManager::m_Instance;

eae6320::Graphics::RenderableManager& eae6320::Graphics::RenderableManager::Get()
{
	return m_Instance;
}

void eae6320::Graphics::RenderableManager::CleanUp()
{
	m_Instance.m_list.clear();
}

void eae6320::Graphics::RenderableManager::Submit(const Renderable& i_renderable)
{
	m_Instance.m_list.push_back(i_renderable);
}