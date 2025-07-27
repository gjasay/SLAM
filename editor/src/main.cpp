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

    //ui
    const auto myPanel = scene->canvas.AddElement(std::make_unique<slam::ui::Panel>(Vector2{300, 150}, 200, 200));
    scene->canvas.styles.AddRule(".myStyle")
        .borderWidth(5)
        .backgroundColor({0, 255, 0, 255})
        .borderColor({0, 0, 0, 255})
        .color({255, 0, 0, 255})
        .fontSize(20)
        .borderRadius(0.1f)
        .flex(true)
        .flexDirection(slam::ui::FlexDirection::Row)
        .flexGap(12);

    myPanel->classes.emplace_back("myStyle");
    myPanel->AddChild(std::make_unique<slam::ui::Text>("poop", Vector2{10, 10}, 60, 60))->classes.emplace_back("myStyle");
    myPanel->AddChild(std::make_unique<slam::ui::Text>("sock", Vector2{10, 10}, 60, 60))->classes.emplace_back("myStyle");
    myPanel->AddChild(std::make_unique<slam::ui::Text>("another option", Vector2{10, 10}, 60, 60))->classes.emplace_back("myStyle");
    myPanel->classes.emplace_back("myStyle");

    engine.SetScene(std::move(scene));
    engine.Run();
}