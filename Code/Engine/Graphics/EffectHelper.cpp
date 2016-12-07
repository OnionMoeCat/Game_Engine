// Header Files
//=============

#include "EffectHelper.h"

#include <assert.h>
#include <sstream>

#include "../UserOutput/UserOutput.h"
#include "../Utils/Utils.h"
#include "../Math/Functions.h"

// Interface
//==========

bool eae6320::Graphics::EffectHelper::LoadEffectFromFile(Effect& i_effect, const char* const i_effectPath)
{
	bool wereThereErrors = false;

	const char* vertexPath = NULL;
	const char* fragmentPath = NULL;
	uint8_t renderStates = 0;

	void* temporaryBuffer = NULL;
	{
		if (eae6320::Utils::ReadDataFromFile(i_effectPath, &temporaryBuffer))
		{
			char* tempPTR = reinterpret_cast<char*>(temporaryBuffer);
			uint8_t strLength = *reinterpret_cast<uint8_t*>(tempPTR);
			tempPTR += sizeof(uint8_t);
			vertexPath = reinterpret_cast<char*>(tempPTR);			
			tempPTR += strLength;

			strLength = *reinterpret_cast<uint8_t*>(tempPTR);
			tempPTR += sizeof(uint8_t);
			fragmentPath = reinterpret_cast<char*>(tempPTR);
			tempPTR += strLength;

			renderStates = *reinterpret_cast<uint8_t*>(tempPTR);
		}
		else
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	i_effect.m_renderStates = renderStates;

	if (!LoadEffectFromFile(i_effect, vertexPath, fragmentPath, Context::Get()))
	{
		wereThereErrors = true;
		goto OnExit;
	}

OnExit:

	renderStates = 0;
	vertexPath = NULL;
	fragmentPath = NULL;

	if (temporaryBuffer)
	{
		free(temporaryBuffer);
	}
	temporaryBuffer = NULL;

	return !wereThereErrors;
}

bool eae6320::Graphics::EffectHelper::CleanUp(Effect& i_effect)
{
	return CleanUp(i_effect, Context::Get());
}