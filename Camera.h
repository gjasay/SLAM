#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameObject.h"

namespace slam
{
	enum ProjectionType
	{
		PERSPECTIVE = CAMERA_PERSPECTIVE,
		ORTHOGRAPHIC = CAMERA_ORTHOGRAPHIC
	};

	//class Camera : public GameObject
	//{
	//public:
	//	Camera(std::string name, Vector3 pos, Vector3 rot = { 0, 1, 0 });

	//	Vector3 Target;
	//	Vector3 Up;
	//	float Fovy;
	//	ProjectionType Projection;
	//	void _Draw() override {}
	//};

	class FPCamera : public GameObject
	{
	public:
		FPCamera(Vector3 pos);

		float Speed;
		float Sensitivity;

		void Update(float dt) override;

		Camera* GetCamera() { return &m_camera; } // Get the camera for rendering

	private:
		Camera m_camera;
		Vector3 m_lookDirection;
	};
}
