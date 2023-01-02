#include "KongEffect.h"

void KongEffect::SetEffect(ID3D11DeviceContext* context)
{
	for (auto i : mShaderList)
	{
		i->UpdateAndSetShader(context);
	}
}

void KongEffect::addShader(IKongShader* shader)
{
	mShaderList.push_back(shader);
}

KongEffect::KongEffect()
{
}
