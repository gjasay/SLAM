#include "../include/Camera.h"

namespace slam {
	FPCamera::FPCamera(Vector3 pos)
		:  GameObject("FirstPersonCamera", pos, {0, 0, 0}), Speed(5.0f), Sensitivity(0.4f)
	{
		m_lookDirection = { 0.0f, 0.0f, -1.0f };

		m_camera.position = pos;
		m_camera.target = m_lookDirection;
		m_camera.up = { 0.0f, 1.0f, 0.0f };
		m_camera.fovy = 60.0f;
		m_camera.projection = CAMERA_PERSPECTIVE;
	}

	void FPCamera::Update(float dt)
	{
		Vector2 mouseDelta = GetMouseDelta();
		Rotation.z += mouseDelta.x * Sensitivity;
		Rotation.y -= mouseDelta.y * Sensitivity;

		if (Rotation.y > 89.0f) Rotation.y = 89.0f;
		if (Rotation.y < -89.0f) Rotation.y = -89.0f;

		float radYaw = Rotation.z * DEG2RAD;
		float radPitch = Rotation.y * DEG2RAD;

		m_lookDirection.x = cosf(radYaw) * cosf(radPitch);
		m_lookDirection.y = sinf(radPitch);
		m_lookDirection.z = sinf(radYaw) * cosf(radPitch);

		m_lookDirection = Vector3Normalize(m_lookDirection);

		Vector3 flatDirection = { m_lookDirection.x, 0.0f, m_lookDirection.z };

		Vector3 right = Vector3CrossProduct(flatDirection, { 0.0f, 1.0f, 0.0f });

		if (IsKeyDown(KEY_W)) {
			m_camera.position = Vector3Add(m_camera.position, Vector3Scale(flatDirection, Speed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_S)) {
			m_camera.position = Vector3Subtract(m_camera.position, Vector3Scale(flatDirection, Speed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_A)) {
			m_camera.position = Vector3Subtract(m_camera.position, Vector3Scale(right, Speed * GetFrameTime()));
		}
		if (IsKeyDown(KEY_D)) {
			m_camera.position = Vector3Add(m_camera.position, Vector3Scale(right, Speed * GetFrameTime()));
		}

		m_camera.target = Vector3Add(m_camera.position, m_lookDirection);

	}
}
