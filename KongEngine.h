#pragma once
#include "EngineObject.h"
#include "WorldObject.h"
#include "DirectXRenderCore.h"
#include "Camera.h"
#include <vector>
#include <string>




class KongEngine
{
public:
	void Initialize(HWND, UINT clientWidth, UINT clientHeight);

	WorldObject AddWorldObject(std::wstring filename);
	void UpdateWorldObject(const WorldObject&);
	void Update(float dt);

	Kong::Camera& GetCamera(); 
private:
	std::vector<EngineObject> mEngineObjects;
	IRenderEngine* mRenderEngine;
};
