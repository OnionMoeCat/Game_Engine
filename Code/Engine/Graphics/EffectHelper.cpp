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

void eae6320::Graphics::EffectHelper::CreateLocalToWorldTransform(Effect& i_effect, const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_vector)
{
	i_effect.m_transform_localToWorld = eae6320::Math::cMatrix_transformation(i_rotation, i_vector);
}

void eae6320::Graphics::EffectHelper::CreateWorldToViewTransform(Effect& i_effect, const eae6320::Math::cQuaternion& i_cameraRotation, const eae6320::Math::cVector& i_cameraPosition)
{
	i_effect.m_transform_worldToView = eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(i_cameraRotation, i_cameraPosition);
}

void eae6320::Graphics::EffectHelper::CreateViewToScreenTransform(Effect& i_effect, const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ)
{
	i_effect.m_transform_viewToScreen = eae6320::Math::cMatrix_transformation::CreateViewToScreenTransform(eae6320::Math::ConvertDegreesToRadians(i_fov), i_aspect, i_nearZ, i_farZ);
}

bool eae6320::Graphics::EffectHelper::LoadEffectFromFile(Effect& i_effect, const char* const i_effectPath)
{
	bool wereThereErrors = false;

	const char* vertexPath = NULL;
	const char* fragmentPath = NULL;

	void* temporaryBuffer = NULL;
	{
		if (eae6320::Utils::ReadDataFromFile(i_effectPath, &temporaryBuffer))
		{
			char* tempPTR = reinterpret_cast<char*>(temporaryBuffer);
			uint8_t strLength = *reinterpret_cast<uint8_t*>(tempPTR);
			tempPTR += sizeof(uint8_t);
			vertexPath = reinterpret_cast<char*>(tempPTR);
			tempPTR += strLength;
			fragmentPath = reinterpret_cast<char*>(tempPTR);
		}
		else
		{
			wereThereErrors = true;
			goto OnExit;
		}
	}

	if (!LoadEffectFromFile(i_effect, vertexPath, fragmentPath, Context::Get()))
	{
		wereThereErrors = true;
		goto OnExit;
	}

OnExit:

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