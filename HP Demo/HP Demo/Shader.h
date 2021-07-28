#pragma once
#include "DX.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"

class Shader {
private:
	__declspec(align(16))
		struct cBufferPerObject
	{
		cBufferPerObject() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMMATRIX worldViewProj;
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX InverseWorld;
	};

	__declspec(align(16))
		struct cBufferCamera {

		DirectX::XMFLOAT3 cameraPosition;
		float padding;
	};

	__declspec(align(16))
		struct cBufferLight
	{
		DirectX::XMFLOAT4 ambientLightColor;
		DirectX::XMFLOAT4 diffuseLightColor;
		DirectX::XMFLOAT4 specularLightColor;
		DirectX::XMFLOAT3 lightPosition;
		float lightRange;

		DirectX::XMFLOAT3 lightAttenuation;
		float padding;
	};

	__declspec(align(16))
		struct cBufferMaterial
	{
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 specularColor;

		int hasTexture;
		int isTerrain;
		int hasNormMap;
		int canMove;
	};

public:
	Shader(ID3D11Device* device);
	~Shader();

	bool InitializeShaders(ID3D11Device* device, HWND hwnd, LPCWSTR vsFilename, LPCWSTR psFilename, LPCSTR entryVS, LPCSTR entryPS);

	bool CreateDefaultInputLayout(ID3D11Device* device);
	bool CreateSkyboxInputLayout(ID3D11Device* device, ID3D11DeviceContext* context);

	bool Render(ID3D11DeviceContext* context, Model* model, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, Camera* camera, Light* light, ID3D11SamplerState* sampler);
	bool RenderWithCubemap(ID3D11DeviceContext* context, Model* model, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView* cubemap, Camera* camera, Light* light, ID3D11SamplerState* sampler);

private:

	bool SetCBuffers(ID3D11DeviceContext* context, Model* model, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, Camera* camera, Light* light);
	bool SetCBuffersWithCubemap(ID3D11DeviceContext* context, Model* model, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView* cubemap, Camera* camera, Light* light);

	void RenderShader(ID3D11DeviceContext*, int, ID3D11SamplerState* sampler);

private:
	HRESULT hr;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;

	ID3DBlob* ErrorBlob;
	ID3DBlob* VSBlob;
	ID3DBlob* PSBlob;

	cBufferPerObject objectCB;
	cBufferCamera cameraCB;
	cBufferLight lightCB;
	cBufferMaterial materialCB;

	ID3D11Buffer* objectBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* materialBuffer;

	ID3D11ShaderResourceView* cubeMap;
	ID3D11ShaderResourceView* normalMapSRV;
	ID3D11ShaderResourceView* texture;

	ID3D11Device* dx11;
};