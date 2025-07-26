#include "../include/ECS.h"
#include "raymath.h"
#include <iostream>

void slam::RenderMeshes(const slam::ECS& ecs) {
    for (const auto& entity : ecs.GetAllEntities()) {
        if (!ecs.HasComponent<Transform>(entity) || !ecs.HasComponent<MeshRenderer>(entity)) {
            continue;
        }

        const auto& transform = ecs.GetComponent<Transform>(entity);
        const auto& meshRenderer = ecs.GetComponent<MeshRenderer>(entity);

        if (!meshRenderer.IsVisible) continue;

        const auto rotationRadians = Vector3{
            transform.Rotation.x * DEG2RAD,
            transform.Rotation.y * DEG2RAD,
            transform.Rotation.z * DEG2RAD
        };

        ::Model mesh = meshRenderer.Mesh;
        mesh.transform = ::MatrixRotateXYZ(rotationRadians);
        DrawModelEx(mesh, transform.Position, {0, 1, 0}, 0, transform.Scale, meshRenderer.Color);

        if (meshRenderer.ShowWires) {
            DrawModelWiresEx(mesh, transform.Position, {0, 1, 0}, 0, transform.Scale, ::PINK);
        }
    }
}