#include <memory>
#include "slam.h"

#include "io/FileSystem.h"

class MyScript final : public slam::Script {
public:
    MyScript(slam::Entity* entity, slam::Scene* scene) : slam::Script(entity, scene) {}


    void Awake() override {
        transform = scene->GetComponent<slam::Transform>(*entity);
        transform->Position = {3, 0, 0};
    }

    void Update(const float dt) override {
        transform->Rotation.x += 50 * dt;
        transform->Rotation.y += 50 * dt;
        transform->Rotation.z += 50 * dt;
    }

private:
  slam::Transform* transform;
};

class MyPanel final : public slam::ui::Panel {
};

int main(int argc, char* argv[])
{
    slam::io::FileSystem::Initialize(argv[0]);

    slam::Engine engine(800, 600, "SLAM Editor");
    auto scene = std::make_unique<slam::Scene>("MainScene");
    auto cube = scene->CreateEntity("");
    const auto cubeRenderer = scene->AddComponent<slam::MeshRenderer>(cube);
    scene->AddComponent<MyScript>(cube);
    cubeRenderer->Mesh = LoadModelFromMesh(GenMeshCube(2, 2, 2));
    cubeRenderer->Color = {255, 0, 0, 255};

    // Load styles from .sss file
    slam::io::StyleSheetParser styleParser(scene->canvas.styles);
    styleParser.Parse(slam::io::FileSystem::GetResourcePath("styles.sss"));

    const auto outerPanel = scene->canvas.AddElement(std::make_unique<slam::ui::Element>(Vector2{0, 0}, 800, 600));
    const auto innerPanel = outerPanel->AddChild(std::make_unique<slam::ui::Panel>(Vector2{50, 20}, 200, 200));
    outerPanel->AddChild(std::make_unique<slam::ui::Panel>(Vector2{50, 20}, 200, 200))->classes.emplace_back("innerStyle");
    outerPanel->id = "outerPanel";
    innerPanel->id = "innerPanel";
    innerPanel->AddChild(std::make_unique<slam::ui::Text>("one", Vector2{0, 0}))->classes.emplace_back("textStyle");
    innerPanel->AddChild(std::make_unique<slam::ui::Text>("two", Vector2{0, 0}))->classes.emplace_back("textStyle");
    innerPanel->AddChild(std::make_unique<slam::ui::Text>("three", Vector2{0, 0}))->classes.emplace_back("textStyle");

    engine.SetScene(std::move(scene));
    engine.Run();
}