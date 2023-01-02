#pragma once
#include <d3d11.h>
#include "EngineObject.h"
#include "KongEffect.h"

class RenderObject
{
public:
	RenderObject(ID3D11Device*, const EngineObject&);

	void ChangeEffect(KongEffect*);
	void Draw(ID3D11DeviceContext*);

	unsigned int GetID() const;
	void SetID(unsigned int id);

	void SetRenderable(bool);

protected:
	//Mesh info
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY mTopology;
	UINT mNumIndices;

	//Texture info
	ID3D11ShaderResourceView* mPSResourceView;

	KongEffect* mEffect;

	unsigned int mObjectID;
	bool mIsRenderable;
};

