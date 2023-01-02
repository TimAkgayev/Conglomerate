#pragma once
#include "Shader.h"
#include <DirectXMath.h>
#include "Camera.h"

class BasicEffect_vs : public IKongShader
{
public:
	BasicEffect_vs(ID3D11Device* device, const Kong::Camera* camera);
	virtual void UpdateAndSetShader(ID3D11DeviceContext*) const override;

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11Buffer* mViewProjectionTransformVariables;
	ID3D11Buffer* mWorldTransofrmVariable;
	ID3D11InputLayout* mInputLayout;

	const Kong::Camera* mCamera;

	void mUpdateShaderVariables(ID3D11DeviceContext*) const;
};

class BasicEffect_ps : public IKongShader
{
public:
	BasicEffect_ps(ID3D11Device*);
	virtual void UpdateAndSetShader(ID3D11DeviceContext*) const override;

private:

	ID3D11PixelShader* mPixelShader;
	ID3D11SamplerState* mPSSampler;

};

