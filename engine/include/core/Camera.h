#pragma once
#include "Entity.h"
#include "Script.h"
#include "raylib.h"
#include "raymath.h"

namespace slam
{
	enum ProjectionType
	{
		PERSPECTIVE = CAMERA_PERSPECTIVE,
		ORTHOGRAPHIC = CAMERA_ORTHOGRAPHIC
	};

	class PlayerCameraController : public Script
	{
	public:
		PlayerCameraController();
		float Speed;
		float Sensitivity;


		PlayerCameraController(Entity *entity, Scene *scene);

		void Awake() override;
		void Update(float dt) override;

		Camera* GetCamera() { return &m_camera; } // Get the camera for rendering

	private:
		Camera m_camera;
		Vector3 m_lookDirection;
		Transform* transform;
	};
}
