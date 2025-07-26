#include "slam.h"
#include <memory>

int main()
{
    slam::Engine engine(800, 600, "SLAM Editor");
    ::DisableCursor();
    std::unique_ptr<slam::Scene> scene = std::make_unique<slam::Scene>("MainScene");
    auto cube = scene->CreateEntity("");
    const auto transform = scene->GetComponent<slam::Transform>(cube);
    const auto cubeRenderer = scene->AddComponent<slam::MeshRenderer>(cube);
    cubeRenderer->Mesh = LoadModelFromMesh(GenMeshCube(2, 2, 2));
    cubeRenderer->Color = {255, 0, 0, 255};
    engine.SetScene(std::move(scene));
    engine.Run();
}