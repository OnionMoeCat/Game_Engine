#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

#include "UserOutput.h"

namespace eae6320
{
	namespace UserOutput
	{
		void Print(std::string i_str)
		{
			OutputDebugStringA(i_str.c_str());
		}
	}
}