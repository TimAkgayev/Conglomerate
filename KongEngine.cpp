#include "KongEngine.h"
#include "ObjectReader.h"
#include "KongEffect.h"

void KongEngine::Initialize(HWND window, UINT clientWidth, UINT clientHeight)
{
	mRenderEngine = new DirectXRenderCore();
	mRenderEngine->Initialize(window, clientWidth, clientHeight);
	
}

WorldObject KongEngine::AddWorldObject(std::wstring filename)
{
	
	
	EngineObject obj = ObjectReader::ExtractSingle(filename);
	obj.effect_id = KONG_EFFECT::BASIC_EFFECT;
	obj.object_id = mEngineObjects.size();
	mEngineObjects.push_back(obj);

	WorldObject o;
	o.mEffect = obj.effect_id;
	o.mObjHandle = (unsigned int) mEngineObjects.size() - 1;
	
	return o;

}

void KongEngine::UpdateWorldObject(const WorldObject& o)
{
	mEngineObjects[o.mObjHandle].effect_id = o.mEffect;
}

void KongEngine::Update(float dt)
{

	for (auto worldObject : mEngineObjects)
		mRenderEngine->SubmitObject(worldObject);

	mRenderEngine->Render();
}

Kong::Camera& KongEngine::GetCamera()	
{
	return mRenderEngine->GetCamera();
}
