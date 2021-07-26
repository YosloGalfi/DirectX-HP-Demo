#pragma once
#include "DX.h"
#include "Texture.h"
#include <vector>
#include <string>

struct Vertex
{
	Vertex() :pos(), texCoord(), normal(), tangent() {}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz), tangent(tz, ty, tz) { }

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
};

// Material for our models
struct SurfaceMaterial
{
	std::wstring materialName;
	DirectX::XMFLOAT4 diffuseColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 ambientColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 specularColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 reflectionColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	float shine = 1;

	bool hasTexture = false;
	bool hasReflection = false;

	int textureArrayIndex = 0;
	bool isTerrain = false;
	bool isTransparent = false;

	DirectX::XMFLOAT4 translation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	bool hasNormalMap = false;
	int normMapTexArrayIndex = 0;
};

class Model {

public:
	Model();
	Model(const Model& other);
	Model(std::string name);
	~Model();

	bool InitializeSkybox(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR textureFilename);
	bool InitWaterQuad(ID3D11Device* device, LPCWSTR textureFilename);

	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetVertexCount();
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetNormalMap();
	std::vector<int>& GetSubsetIndexVector();
	std::vector<int>& GetSubsetMaterialVector();
	int& GetSubsetCount();
	std::vector<DWORD>& GetIndices();
	std::vector<XMFLOAT3>& GetVerticesArray();
	std::vector<Vertex>& GetVertices();
	std::vector<SurfaceMaterial>& GetMaterial();
	std::vector<std::wstring>& GetTextureNameVector();
	Texture* GetTextureStruct() { return this->texture; }
	ID3D11Buffer& GetVertexBuffer() { return *this->vertexBuffer; }
	ID3D11Buffer& GetIndexBuffer() { return *this->indexBuffer; }
	DirectX::XMMATRIX GetWorldMatrix() { return this->world; }

	ID3D11ShaderResourceView* GetCubeMap() { return this->cubemapTexture->GetTexture(); }
	bool SetCubemapTexture(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR textureFilename);

	void SetTexture(ID3D11ShaderResourceView* resource);
	void SetIndexBuffer(ID3D11Buffer* indexBuffer);
	void SetVertexBuffer(ID3D11Buffer* vertexBuffer);
	void SetWorldMatrix(DirectX::XMMATRIX world) { this->world = world; }
	void SetVertexCount(int count) { this->vertexCount = count; }
	void SetIndexCount(int count) { this->indexCount = count; }

	/* Texture loading */
	bool LoadTexture(ID3D11Device*, LPCWSTR);
	bool LoadNormalMap(ID3D11Device*, LPCWSTR);
	void LoadNormalMapFbx(Texture* tex) { this->normalMap = tex; }
	void LoadFbxTexture(Texture* tex) { this->texture = tex; }

	//bool InitializeFromFbx(std::vector<Vertex> vertices, std::vector<DWORD> indices, Skeleton* skeleton, ID3D11Device* device);
	bool InitializeFromFbx(std::vector<Vertex> vertices, std::vector<DWORD> indices, ID3D11Device* device);

private:
	void ShutdownBuffers();
	void ReleaseTexture();
	bool CreateQuad(ID3D11Device*);

private:
	HRESULT hr;
	ID3D11Buffer* vertexBuffer, * indexBuffer;
	int vertexCount, indexCount;

	Texture* cubemapTexture;
	Texture* texture;
	Texture* normalMap;
	std::string modelName;

	std::vector<XMFLOAT3> vertPosArray;			// Used for CPU to do calculations on the Geometry
	std::vector<DWORD> vertIndexArray;			// Also used for CPU calculations on geometry

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::vector<int> subsetIndexStart;
	std::vector<int> subsetMaterials;
	std::vector<SurfaceMaterial> materials;
	std::vector<std::wstring> textureNames;
	int subsetCount;

	DirectX::XMMATRIX world;
};