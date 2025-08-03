#include <memory>
#include "slam.h"

#include "io/FileSystem.h"

class MyScript final : public slam::Script {
public:
  MyScript(slam::Entity *entity, slam::Scene *scene) : slam::Script(entity, scene) {}


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
  slam::Transform *transform;
};

class MyPanel final : public slam::ui::Panel {
public:
    void OnCreate() override {
      this->AddClass("panelStyle");
      this->AddChild(std::make_unique<slam::ui::Text>("one"))->AddClass("textStyle");
      this->AddChild(std::make_unique<slam::ui::Text>("two"))->AddClass("textStyle");
      this->AddChild(std::make_unique<slam::ui::Text>("three"))->AddClass("textStyle");
      this->inlineStyle->width = 200;
      this->inlineStyle->height = 100;
      this->inlineStyle->backgroundColor = {0, 255, 0, 255};
      this->inlineStyle->borderColor = {255, 0, 0, 255};
      this->inlineStyle->borderWidth = 2;
      this->inlineStyle->borderRadius = 0.1f;
    }
};

int main(int argc, char *argv[]) {
  slam::io::FileSystem::Initialize(argv[0]);

  ::SetConfigFlags(::FLAG_WINDOW_RESIZABLE);

  slam::Engine engine(1600, 900, "SLAM Editor");
  auto scene = std::make_unique<slam::Scene>("MainScene", 1600, 900);
  auto cube = scene->CreateEntity("");
  const auto cubeRenderer = scene->AddComponent<slam::MeshRenderer>(cube);
  scene->AddComponent<MyScript>(cube);
  cubeRenderer->Mesh = LoadModelFromMesh(GenMeshCube(2, 2, 2));
  cubeRenderer->Color = {255, 0, 0, 255};

  // Load styles from .sss file
  slam::io::StyleSheetParser styleParser(scene->canvas.styles);
  styleParser.Parse(slam::io::FileSystem::GetResourcePath("styles.sss"));

  scene->canvas.AddElement(std::make_unique<slam::ui::Slider>())->AddClass("sliderStyle");
  scene->canvas.AddElement(std::make_unique<MyPanel>());

  engine.SetScene(std::move(scene));
  engine.Run();
}
