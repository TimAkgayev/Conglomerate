#pragma once
#include <windows.h>
#include "EngineObject.h"
#include "Camera.h"

class IRenderEngine 
{
public:

	virtual UINT Initialize(HWND window, UINT clientWidht, UINT clientHeight) = 0; 
	virtual void SubmitObject(const EngineObject&) = 0;
	virtual UINT Render() = 0;
	virtual Kong::Camera& GetCamera() = 0;

};