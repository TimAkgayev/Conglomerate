#pragma once
#include "KongEffect.h"
#include "Camera.h"

namespace KongEffectLibrary
{

	class BasicEffect : public KongEffect
	{
	public:
		BasicEffect(ID3D11Device* device, const Kong::Camera*);
	};

}