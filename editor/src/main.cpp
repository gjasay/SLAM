#include "slam.h"
#include <memory>

class MyScript final : public slam::Script {
public:
    MyScript(slam::Entity* entity, slam::Scene* scene) : slam::Script(entity, scene) {}

    void Awake() override {
        // std::cout << "MyScript: Awake called for entity " << entity->name << std::endl;
    }

    void Update(float dt) override {
        // std::cout << "MyScript: Update called for entity " << entity->name << " with dt = " << dt << std::endl;
    }
};

int main()
{
    slam::Engine engine(800, 600, "SLAM Editor");
    auto scene = std::make_unique<slam::Scene>("MainScene");
    auto cube = scene->CreateEntity("");
    const auto cubeRenderer = scene->AddComponent<slam::MeshRenderer>(cube);
    scene->AddComponent<MyScript>(cube);
    cubeRenderer->Mesh = LoadModelFromMesh(GenMeshCube(2, 2, 2));
    cubeRenderer->Color = {255, 0, 0, 255};

    //ui
    scene->canvas.AddElement(std::make_unique<slam::ui::Panel>(Vector2{0, 0}, 200, 200));
    engine.SetScene(std::move(scene));
    engine.Run();
}