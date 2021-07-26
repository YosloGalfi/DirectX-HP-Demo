#pragma once
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <assert.h>

#include "Scene.h"
#include "Timer.h"

#define DEFAULT_SCREEN_WIDTH 2560	
#define DEFAULT_SCREEN_HEIGHT 1440

class System {
public:
	System(HINSTANCE hInstance);
	~System();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	bool Initialize();
	void Shutdown();
	void Run();

private:
	bool UpdateOnFrame();
	void InitializeWindows(int& width, int& height);
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	const wchar_t* projectTitel = L"Harry Potter Features showcase";
	int screenWidth, screenHeight;
	HINSTANCE hInstance;
	HWND hwnd;
	HRESULT hr;

	Scene* scene;
	Timer fpsTimer;

	int frameCount = 0;
	double frameTime = 0;
};

static System* appHandle;