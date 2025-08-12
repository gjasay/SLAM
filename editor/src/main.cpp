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

  // Load styles from .sss file
  slam::io::StyleSheetParser styleParser(scene->canvas.styles);
  styleParser.Parse(slam::io::FileSystem::GetResourcePath("styles.sss"));

  auto* sliderr = dynamic_cast<slam::ui::Slider*>(scene->canvas.AddElement(std::make_unique<slam::ui::Slider>()));
  sliderr->AddClass("sliderStyle");
  sliderr->id = "slideerrr";

  scene->canvas.AddElement(std::make_unique<slam::ui::Button>("Click Me"))->AddClass("buttonStyle");

  for (int i = 0; i < 5; i++) {
    auto* container = scene->canvas.AddElement(std::make_unique<slam::ui::Element>());
    container->id = "container" + std::to_string(i);
    auto* slider = dynamic_cast<slam::ui::Slider*>(container->AddChild(std::make_unique<slam::ui::Slider>()));
    auto* text = dynamic_cast<slam::ui::Text*>(container->AddChild(std::make_unique<slam::ui::Text>("0")));
    container->inlineStyle->flex = true;
    container->inlineStyle->flexDirection = slam::ui::FlexDirection::Row;
    container->inlineStyle->justifyContent = slam::ui::JustifyContent::SpaceBetween;
    container->inlineStyle->alignItems = slam::ui::AlignItems::Center;
    container->inlineStyle->widthIsPercent = true;
    container->inlineStyle->widthPercent = 0.75f;
    container->AddClass("container");
    slider->AddClass("sliderStyle");
    slider->id = "slider" + std::to_string(i);
    text->AddClass("textStyle");
    text->id = "text" + std::to_string(i);
    slider->OnValueChange = [text](float value) {
      text->InnerText = std::to_string(static_cast<int>(value * 100));
    };
  }

  scene->canvas.AddElement(std::make_unique<slam::ui::ExpandableTextInput>());

  engine.SetScene(std::move(scene));
  engine.Run();
}
