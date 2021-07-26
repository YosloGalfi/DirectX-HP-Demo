#pragma once
#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(HWND hwnd);
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetTargetPosition(float x, float y, float z);
	void SetUpvector(float x, float y, float z);

	DirectX::XMFLOAT3 GetLookAt();
	DirectX::XMFLOAT3 GetPosition();

	void SetViewMatrix(XMVECTOR eye, XMVECTOR lookAt, XMVECTOR upVec);
	void GetViewMatrix(DirectX::XMMATRIX&);

	void UpdateCamera();


private:
	HWND hwnd;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX camRotationMatrix;

	// Mouse x and y-axis position
	float camYaw = 0.0f;
	float camPitch = 0.0f;

	// Keyboard "wasd" movement
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
};