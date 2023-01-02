#include "Main.h"
#include "ObjectReader.h"


#ifdef _KONG_WINDOWS_
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	MyApp app;

	app.CreateAppWindow(hInstance, 800, 600, L"Konglomerate");

	return app.RunLoop();
}


LRESULT MyApp::MessageProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer.stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.start();
		}
		return 0;
	}


	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}

	return DefWindowProc(mMainWindow, msg, wParam, lParam);

}

void MyApp::updateCamera(float dt)
{

	Kong::Camera& cam = mEngine.GetCamera();

	XMVECTOR moveDirLook = XMVectorSet(cam.GetLookAtVector().x, 0, cam.GetLookAtVector().z, 1.0f);
	XMVECTOR moveDirRight = XMVectorSet(cam.GetRightVector().x, 0, cam.GetRightVector().z, 1.0f);
	XMVECTOR moveDirUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	if (GetAsyncKeyState(0x57)) //W key
		cam.Move(dt * moveDirLook * -1 * mMoveSpeed);

	if (GetAsyncKeyState(0x53)) //S key
		cam.Move(dt * moveDirLook * mMoveSpeed);


	if (GetAsyncKeyState(0x41)) //A key
		cam.Move(dt * moveDirRight * -1 * mMoveSpeed);


	if (GetAsyncKeyState(0x44)) //D key
		cam.Move(dt * moveDirRight * mMoveSpeed);



	if (GetAsyncKeyState(VK_UP))
		cam.Pitch(-1 * dt * mTurnSpeed);


	if (GetAsyncKeyState(VK_DOWN))
		cam.Pitch(1 * dt * mTurnSpeed);


	if (GetAsyncKeyState(VK_LEFT))
		cam.Yaw(-1 * dt * mTurnSpeed);


	if (GetAsyncKeyState(VK_RIGHT))
		cam.Yaw(1 * dt * mTurnSpeed);


	//page up and page down
	if (GetAsyncKeyState(VK_PRIOR))
		cam.Move(dt * moveDirUp * mMoveSpeed);

	if (GetAsyncKeyState(VK_NEXT))
		cam.Move(dt * -1 * moveDirUp * mMoveSpeed);


	
}


#endif


void MyApp::Initialize()
{
	mEngine.Initialize(mMainWindow, mClientWidth, mClientHeight);
	mEngine.AddWorldObject(L"C:\\Users\\Tim\\Documents\\Visual Studio Projects\\Konglomerate\\assets\\FlatTerrain.obj");

	XMFLOAT3 camPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEngine.GetCamera().SetPosition(camPos);

	mMoveSpeed = 10.0f;
	mTurnSpeed = 200.0f;
}

void MyApp::Update(float dt)
{
	mEngine.Update(dt);
	updateCamera(dt);
}
