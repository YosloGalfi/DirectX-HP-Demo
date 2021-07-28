#pragma once
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assert.h>
#include <WICTextureLoader.h>
#include "Model.h"
#include <string>

using namespace DirectX;
using namespace std;

class objLoader
{
private:
	HRESULT hr;
public:
	objLoader();
	~objLoader();

	bool loadObj(Model* model, ID3D11Device* device, wstring fileName, bool isRightHanded, bool computeNormals);
};