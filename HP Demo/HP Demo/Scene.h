#pragma once
#include "DX.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Terrain.h"
#include "objLoader.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Scene
{

private:
	DX11* dx11;
	HRESULT hr;

	Camera* camera;
	Light* light;
	objLoader objLoader;

	Shader* shader;
	Shader* skyboxShader;

	int screenWidth, screenHeight;
	
	Model* skybox;
	Terrain* terrain;
	std::vector<Model*> allModels;

	bool Render();

public:
	Scene();
	~Scene();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void InitializeTerrain(HWND hwnd);
	bool InitializeSkybox(HWND hwnd);

	bool RenderFrame(float deltaTime);

	void Update(float deltaTime);

	/*  CUBEMAPPING */
	/*bool InitializeCubeMapViews(ID3D11Device* device);
	bool BuildCubeFaceCameras(float x, float y, float z);
	bool RenderCubemapping();*/
};