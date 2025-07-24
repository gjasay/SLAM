#include "slam.h"
#include <memory>

int main()
{
    slam::Engine engine(800, 600, "SLAM Editor");
    DisableCursor();
    std::unique_ptr<slam::Cube> cube = std::make_unique<slam::Cube>("MyCube", Vector3{0, 0, 0}, 1.0f, Color{255, 0, 0, 255});
    std::unique_ptr<slam::Cube> cube2 = std::make_unique<slam::Cube>("Cube2", Vector3{0, 0, 0}, 1.0f, Color{255, 0, 0, 255});
    std::unique_ptr<slam::Scene> scene = std::make_unique<slam::Scene>("MainScene", std::make_unique<slam::FPCamera>(Vector3{0, 0, 0}), 400, 600);
    std::unique_ptr<slam::Scene> scene2 = std::make_unique<slam::Scene>("Scene2", std::make_unique<slam::FPCamera>(Vector3{0, 0, 0}), 400, 600);
    scene->Position = { 0, 0 };
    scene2->Position = { 400, 0 };
    scene->AddGameObject(std::move(cube));
    scene2->AddGameObject(std::move(cube2));
    engine.SetScene(std::move(scene));
    engine.AddScene(std::move(scene2));
    engine.Run();
}