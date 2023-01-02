#include "RenderObject.h"
#include <WICTextureLoader.h>
#include "Utility.h"
#include "EffectLibrary.h"

RenderObject::RenderObject(ID3D11Device* device, const EngineObject& obj) : mEffect(nullptr)
{
	//create the index buffer
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(DWORD) * (UINT)obj.index_list.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData = { 0 };
	InitData.pSysMem = &obj.index_list[0];
	mNumIndices = (UINT)obj.index_list.size();
	device->CreateBuffer(&bufferDesc, &InitData, &mIndexBuffer);

	//create the vertex buffer
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * (UINT)obj.vertex_list.size(); //total size of buffer in bytes
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &obj.vertex_list[0];
	device->CreateBuffer(&bufferDesc, &InitData, &mVertexBuffer);


	//define topology
	mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//load texture
	DirectX::CreateWICTextureFromFile(device, obj.texture_path.c_str(), NULL, &mPSResourceView);

	mObjectID = obj.object_id;
	mIsRenderable = true;
}

void RenderObject::ChangeEffect(KongEffect* e)
{
	mEffect = e;
}

void RenderObject::Draw(ID3D11DeviceContext* context)
{

	if (!mIsRenderable)
		return;

	//set our mesh resource
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(mTopology);

	//set the texture resource
	context->PSSetShaderResources(0, 1, &mPSResourceView);

	//set other shaders
	mEffect->SetEffect(context);

	context->DrawIndexed(mNumIndices, 0, 0);
}

unsigned int RenderObject::GetID() const
{
	return mObjectID;
}

void RenderObject::SetID(unsigned int id)
{
	mObjectID = id;
}

void RenderObject::SetRenderable(bool state)
{
	mIsRenderable = state;
}

