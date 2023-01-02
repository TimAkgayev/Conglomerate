#pragma once
#include "Timer.h"

class Application
{
public: 
	Application() :mAppPaused(false), mClientWidth(0), mClientHeight(0) {}


	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;

protected:

	bool  mAppPaused;
	Timer mTimer;

	unsigned int mClientWidth;
	unsigned int mClientHeight;
};