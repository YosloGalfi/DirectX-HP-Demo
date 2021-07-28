#pragma once
#include "Model.h"
#include <DirectXMath.h>
#include <string>

class Terrain
{
private:
	Model* mesh;

	// terrain width + height
	int width, height;

	// cellSpace, is used if you want to create a grid over the whole terrain and how big you want it to be
	float cellSpace;

public:
	Terrain();
	~Terrain();

	// Get the mesh which the terrain is based on
	Model* GetMesh() { return this->mesh; }

	// Returns the height of a triangle at the given X and Z coordinates
	float GetTriangleHeight(const float x, const float z);

	// Loads a height map and creates the terrain
	void CreateTerrain(std::string filename, ID3D11Device* device, HWND hwnd);

};