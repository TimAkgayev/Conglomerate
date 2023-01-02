#include "ShaderLibrary.h"
#include "Utility.h"
#include "Vertex.h"

BasicEffect_vs::BasicEffect_vs(ID3D11Device* device, const Kong::Camera* camera):mViewProjectionTransformVariables(nullptr), mWorldTransofrmVariable(nullptr)
{

	ID3DBlob* blob;
	compileAndCreateShader(device, L"shaders//vs_transform.hlsl", "vs_transform", &mVertexShader, &blob);

	D3D11_INPUT_ELEMENT_DESC* VertexLayoutDescription = new D3D11_INPUT_ELEMENT_DESC[4];
	VertexLayoutDescription[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	VertexLayoutDescription[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	VertexLayoutDescription[2] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	VertexLayoutDescription[3] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	device->CreateInputLayout(VertexLayoutDescription, NUM_MESH_VERTEX_ELEMENTS, blob->GetBufferPointer(), blob->GetBufferSize(), &mInputLayout);
	ReleaseCOM(blob);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX)*2;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	device->CreateBuffer(&bufferDesc, NULL, &mViewProjectionTransformVariables);

	bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	device->CreateBuffer(&bufferDesc, NULL, &mWorldTransofrmVariable);

	mCamera = camera;
}

void BasicEffect_vs::UpdateAndSetShader(ID3D11DeviceContext* context) const
{
	mUpdateShaderVariables(context);

	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &mViewProjectionTransformVariables);
	context->VSSetConstantBuffers(1, 1, &mWorldTransofrmVariable);
}

void BasicEffect_vs::mUpdateShaderVariables(ID3D11DeviceContext* context) const
{

	DirectX::XMMATRIX vars[2] = { DirectX::XMMatrixTranspose(mCamera->GetViewMatrix()), DirectX::XMMatrixTranspose(mCamera->GetPerspectiveProjectionMatrix()) };
	context->UpdateSubresource(mViewProjectionTransformVariables, 0, NULL, vars, 0, 0);

	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
	context->UpdateSubresource(mWorldTransofrmVariable, 0, NULL, &WorldTransform, 0, 0);
}

BasicEffect_ps::BasicEffect_ps(ID3D11Device* device)
{
	compileAndCreateShader(device, L"shaders//ps_texture.hlsl", "ps_texture", &mPixelShader, nullptr);

	//create a sampler
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	HandleError(device->CreateSamplerState(&samplerDesc, &mPSSampler));
}

void BasicEffect_ps::UpdateAndSetShader(ID3D11DeviceContext* context) const
{
	context->PSSetSamplers(0, 1, &mPSSampler);
	context->PSSetShader(mPixelShader, nullptr, 0);
}
