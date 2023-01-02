#pragma once
#include "Shader.h"
#include <vector>

enum KONG_EFFECT {BASIC_EFFECT};

class KongEffect
{
public:
	virtual void SetEffect(ID3D11DeviceContext* context);

protected:
	KongEffect();
	void addShader(IKongShader*);
	std::vector<IKongShader*> mShaderList;
};