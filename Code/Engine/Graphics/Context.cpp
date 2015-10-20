// Header Files
//=============

#include "Context.h"

// Interface
//==========

eae6320::Graphics::Context eae6320::Graphics::Context::m_Instance;

eae6320::Graphics::Context& eae6320::Graphics::Context::Get()
{
	return m_Instance;
}