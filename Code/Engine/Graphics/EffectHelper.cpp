#include "EffectHelper.h"

bool eae6320::Graphics::EffectHelper::LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath)
{
	return LoadEffectFromFile(i_effect, i_vertexPath, i_fragmentPath, Context::Get());
}
bool eae6320::Graphics::EffectHelper::CleanUp(Effect& i_effect)
{
	return CleanUp(i_effect, Context::Get());
}