#include <stdarg.h>
#include <stdio.h>

#include "UserOutput.h"

// Static Data Initialization
//===========================

namespace
{
	HWND s_renderingWindow = NULL;
}

void eae6320::UserOutput::Print(std::string i_errorMessage, std::string i_errorCaption /*= "Error!"*/)
{
	if (s_renderingWindow)
	{
		MessageBox(s_renderingWindow, i_errorMessage.c_str(), i_errorCaption.c_str(), MB_OK | MB_ICONERROR);
	}
}

void eae6320::UserOutput::Initialize(const HWND& i_renderingWindow)
{
	s_renderingWindow = i_renderingWindow;
}

void eae6320::UserOutput::Finalize()
{
	s_renderingWindow = NULL;
}