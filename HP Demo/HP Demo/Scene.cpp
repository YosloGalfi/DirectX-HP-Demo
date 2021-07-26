#include "Scene.h"

Scene::Scene() {

	this->screenHeight = 0;
	this->screenWidth = 0;
	this->dx11 = 0;
	this->hr = 0;
	this->camera = 0;
	this->model = 0;
	this->shader = 0;
	this->skyboxShader = 0;
	this->skybox = 0;
	this->light = 0;

	this->water = 0;
}

Scene::~Scene()
{
}

bool Scene::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	/*
		Initialize DX11 and set screenwidth and height for later use in picking.
	*/

	dx11 = new DX11;
	if (!dx11)
		return false;

	result = dx11->Initialize(screenWidth, screenHeight, hwnd, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
		return false;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;


	/*
		Create a new main camera.
		Startposition for camera and give camera inputdevice settings.
	*/

	camera = new Camera(hwnd);
	if (!camera)
		return false;

	camera->SetPosition(10.0f, 100.0f, 10.0f);

	/*
		Create light to give to cbuffer.
	*/

	light = new Light;
	// Initialize the light object.
	light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetLightPosition(-5.0f, 30.0f, 30.0f);
	light->SetLightAttentuation(1.0f, 0.02f, 0.0f);
	light->SetLightRange(2000.0f);

	/*
		Init different shaders
	*/

	/* Default shader */
	//shader = new Shader(dx11->GetDevice());
	//result = shader->InitializeShaderWithGeometryShader(dx11->GetDevice(), hwnd, L"Shaders/VSDefault.hlsl", L"Shaders/GSDefault.hlsl", L"Shaders/PSDefault.hlsl", "VSMain", "GSMain", "PSMain");
	//if (!result)
	//	return false;
	/*result = shader->CreateDefaultInputLayout(dx11->GetDevice());
	if (!result)
		return false;*/


	/* Skybox shader */
	skyboxShader = new Shader(dx11->GetDevice());
	result = skyboxShader->InitializeShaders(dx11->GetDevice(), hwnd, L"Shaders/SkyVS.hlsl", L"Shaders/SkyPS.hlsl", "SkyVSMain", "SkyPSMain");
	if (!result)
		return false;
	result = skyboxShader->CreateSkyboxInputLayout(dx11->GetDevice(), dx11->GetContext());
	if (!result)
		return false;


	/*skybox = new Model;
	newReader.loadObj(skybox, dx11->GetDevice(), L"skysphere.obj", true, false);
	skybox->GetMaterial()[0].diffuseColor = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	skybox->GetMaterial()[0].hasTexture = true;*/

	/*result = skybox->InitializeSkybox(dx11->GetDevice(), dx11->GetContext(), L"Textures/skymap.dds");
	if (!result)
		return false;

	skybox->GetTextureStruct()->SetName("Skybox");
	skybox->SetWorldMatrix(XMMATRIX(XMMatrixScaling(75.0f, 75.0f, 75.0f) * XMMatrixTranslation(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z)));*/


	return true;
}

void Scene::Shutdown()
{
	if (camera) {
		delete camera;
		camera = 0;
	}

	if (dx11) {
		dx11->Shutdown();
		delete dx11;
		dx11 = 0;
	}

	if (allModels.size() > 0) {
		for (int i = 0; i < allModels.size(); i++)
		{
			allModels[i]->Shutdown();
			delete allModels[i];
			allModels[i] = 0;
		}
		allModels.clear();
	}

	if (skyboxShader)
	{
		skyboxShader->Shutdown();
		delete skyboxShader;
		skyboxShader = 0;
	}

	if (skybox)
	{
		skybox->Shutdown();
		delete skybox;
		skybox = 0;
	}

	if (shader) {
		shader->Shutdown();
		delete shader;
		shader = 0;
	}

	if (light)
	{
		delete light;
		light = 0;
	}
}

bool Scene::RenderFrame(float deltaTime)
{
	bool result;

	/* Update models suff */
	Update(deltaTime);

	// Render frame
	result = Render();
	if (!result)
		return false;

	return true;
}

void Scene::Update(float deltaTime)
{
	/*
		Skybox follow cam
	*/

	rotSky += 0.02f * (float)deltaTime;
	if (rotSky > 6.28f)
		rotSky = 0.0f;

	XMVECTOR skyrotaxis = DirectX::XMVectorSet(0.0f, rotSky, 0.0f, 0.0f);
	XMMATRIX skycubeRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(skyrotaxis);
	XMMATRIX skyboxScale = XMMatrixScaling(75.0f, 75.0f, 75.0f);

	// FIX
	//skybox->SetWorldMatrix(skyboxScale * skycubeRotation * XMMATRIX(XMMatrixTranslation(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z)));
}

bool Scene::Render()
{
	// RENDER STUFF HERE
	bool result;
	DirectX::XMMATRIX view, projection;

	dx11->BeginScene(0.0f, 0.8f, 0.2f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(view);
	dx11->GetProjectionMatrix(projection);


	/* Rest of the models here with default shader*/
	for (int i = 0; i < allModels.size(); i++) {
		allModels[i]->Render(dx11->GetContext());
		result = shader->Render(dx11->GetContext(), allModels[i], view, projection, camera, light, dx11->GetMinMagMipSampler());
		if (!result)
			return false;
	}

	// FIX
	/*Skybox render alone with skybox shader*/
	//skybox->Render(dx11->GetContext());
	//result = skyboxShader->Render(dx11->GetContext(), skybox, view, projection, camera, light, dx11->GetMinMagMipSampler());
	//if (!result)
	//	return false;


	dx11->EnableAlphaBlending();

	dx11->DisableAlphaBlending();

	dx11->EndScene();
	return true;
}
