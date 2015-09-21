#ifndef USER_OUTPUT_H
#define USER_OUTPUT_H

// Header Files
//=============
#include "../Windows/Includes.h"
#include <string>

// Interface
//=============

namespace eae6320
{
	namespace UserOutput
	{
		void Initialize(const HWND& i_renderingWindow);
		void Print(std::string i_errorMessage, std::string i_errorCaption = "Error!");
		void Finalize();
	}
}
#endif