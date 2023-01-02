#pragma once
#include <windows.h>
#include "WindowsApplication.h"
#include "KongEngine.h"
#include "Camera.h"

#define _KONG_WINDOWS_ 

#ifdef _KONG_WINDOWS_
class MyApp : public WindowsApplication
#else
class MyApp : public LinuxApplication 
#endif
{
public:

	virtual void Initialize() override;
	virtual void Update(float dt) override;

#ifdef _KONG_WINDOWS_
	virtual LRESULT MessageProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
#endif

private:
	void updateCamera(float dt);


	KongEngine mEngine;

	float mMoveSpeed;
	float mTurnSpeed;

	WorldObject mTerrainObject;
};
