#pragma once
#include <Windows.h>
#include <string>
#include "Application.h"


class WindowsApplication : public Application
{
public:
	WindowsApplication();
	WindowsApplication(const WindowsApplication& rhs) = delete;
	WindowsApplication& operator=(const WindowsApplication& rhs) = delete;
	virtual ~WindowsApplication();

	virtual LRESULT MessageProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	int RunLoop();

	HWND CreateAppWindow(HINSTANCE hInstance, UINT width, UINT height, const std::wstring& caption);


protected:
	HWND mMainWindow;
};
