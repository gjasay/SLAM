#include "../../include/core/Camera.h"
#include <stdexcept>
#include "../../include/core/Scene.h"
#include "raylib.h"
#include "raymath.h"

namespace slam {
    PlayerCameraController::PlayerCameraController(Entity* entity, Scene* scene)
        : Script(entity, scene), Speed(5.0f), Sensitivity(0.4f), m_lookDirection({0.0f, 0.0f, -1.0f}) {

        if (!entity || !scene) {
            throw std::runtime_error("PlayerCameraController: entity or scene is not initialized.");
        }

        transform = this->scene->GetComponent<Transform>(*this->entity);
        m_camera.position = transform->Position;
        m_camera.target = Vector3Add(transform->Position, m_lookDirection);
        m_camera.up = {0.0f, 1.0f, 0.0f};
        m_camera.fovy = 60.0f;
        m_camera.projection = CAMERA_PERSPECTIVE;
    }

    void PlayerCameraController::Awake() {}

    void PlayerCameraController::Update(const float dt) {
        Vector2 mouseDelta = GetMouseDelta();
        transform->Rotation.z += mouseDelta.x * Sensitivity;
        transform->Rotation.y -= mouseDelta.y * Sensitivity;

        if (transform->Rotation.y > 89.0f) transform->Rotation.y = 89.0f;
        if (transform->Rotation.y < -89.0f) transform->Rotation.y = -89.0f;

        const float radYaw = transform->Rotation.z * DEG2RAD;
        const float radPitch = transform->Rotation.y * DEG2RAD;

        m_lookDirection.x = cosf(radYaw) * cosf(radPitch);
        m_lookDirection.y = sinf(radPitch);
        m_lookDirection.z = sinf(radYaw) * cosf(radPitch);

        m_lookDirection = Vector3Normalize(m_lookDirection);

        const Vector3 flatDirection = {m_lookDirection.x, 0.0f, m_lookDirection.z};
        const Vector3 right = Vector3CrossProduct(flatDirection, {0.0f, 1.0f, 0.0f});

        if (IsKeyDown(KEY_W)) {
            m_camera.position = Vector3Add(m_camera.position, Vector3Scale(flatDirection, Speed * dt));
        }
        if (IsKeyDown(KEY_S)) {
            m_camera.position = Vector3Subtract(m_camera.position, Vector3Scale(flatDirection, Speed * dt));
        }
        if (IsKeyDown(KEY_A)) {
            m_camera.position = Vector3Subtract(m_camera.position, Vector3Scale(right, Speed * dt));
        }
        if (IsKeyDown(KEY_D)) {
            m_camera.position = Vector3Add(m_camera.position, Vector3Scale(right, Speed * dt));
        }

        m_camera.target = Vector3Add(m_camera.position, m_lookDirection);
    }
}
