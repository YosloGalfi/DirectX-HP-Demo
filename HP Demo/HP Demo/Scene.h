#pragma once
#include "DX.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Scene
{
public:
	Scene();
	~Scene();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool RenderFrame(float deltaTime);

	void Update(float deltaTime);

	/*  CUBEMAPPING */
	/*bool InitializeCubeMapViews(ID3D11Device* device);
	bool BuildCubeFaceCameras(float x, float y, float z);
	bool RenderCubemapping();*/

private:
	bool Render();

private:
	DX11* dx11;
	Camera* camera;
	Model* model;
	Model* skybox;

	int screenWidth, screenHeight;
	HRESULT hr;

	Model* water;

	Shader* shader;
	Shader* skyboxShader;

	Light* light;

	std::vector<Model*> allModels;

	float rotSky = 0.0f;
};