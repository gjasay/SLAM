#include "slam.h"
#include <memory>

class MyScript final : public slam::Script {
public:
    MyScript(slam::Entity* entity, slam::Scene* scene) : slam::Script(entity, scene) {}


    void Awake() override {
        transform = scene->GetComponent<slam::Transform>(*entity);
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

int main()
{
    slam::Engine engine(800, 600, "SLAM Editor");
    auto scene = std::make_unique<slam::Scene>("MainScene");
    auto cube = scene->CreateEntity("");
    const auto cubeRenderer = scene->AddComponent<slam::MeshRenderer>(cube);
    scene->AddComponent<MyScript>(cube);
    cubeRenderer->Mesh = LoadModelFromMesh(GenMeshCube(2, 2, 2));
    cubeRenderer->Color = {255, 0, 0, 255};

    scene->canvas.styles.AddRule(".textStyle")
        .color({255, 0, 0, 255})
        .fontSize(20);

    scene->canvas.styles.AddRule("#innerPanel")
        .borderWidth(5)
        .backgroundColor(::GRAY)
        .borderColor({0, 0, 0, 255})
        .flexDirection(slam::ui::FlexDirection::Row)
        .justifyContent(slam::ui::JustifyContent::SpaceBetween)
        .alignItems(slam::ui::AlignItems::Center)
        .borderRadius(1.0f)
        .flex(true);

    scene->canvas.styles.AddRule("#outerPanel")
        .visible(false);

    const auto outerPanel = scene->canvas.AddElement(std::make_unique<slam::ui::Element>(Vector2{0, 0}, 800, 600));
    const auto innerPanel = outerPanel->AddChild(std::make_unique<slam::ui::Panel>(Vector2{50, 20}, 200, 200));
    outerPanel->id = "outerPanel";
    innerPanel->id = "innerPanel";
    innerPanel->AddChild(std::make_unique<slam::ui::Text>("one", Vector2{0, 0}))->classes.emplace_back("textStyle");
    innerPanel->AddChild(std::make_unique<slam::ui::Text>("two", Vector2{0, 0}))->classes.emplace_back("textStyle");
    innerPanel->AddChild(std::make_unique<slam::ui::Text>("three", Vector2{0, 0}))->classes.emplace_back("textStyle");

    engine.SetScene(std::move(scene));
    engine.Run();
}