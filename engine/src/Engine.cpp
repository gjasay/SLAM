#include "../include/Engine.h"
#include <iostream>

namespace slam {
	void Engine::SetScene(std::unique_ptr<Scene> scene) {
		if (!scene) {
			std::cerr << "Attempted to set a null Scene!" << std::endl;
			return;
		}

		m_activeScenes.clear();
		m_sceneMap.clear();
		scene->_setEngine(this);
		m_sceneMap[scene->Name] = scene.get();
		m_activeScenes.push_back(std::move(scene));
	}

	void Engine::AddScene(std::unique_ptr<Scene> scene) {
		if (!scene) { std::cerr << "Attempted to add a null Scene!" << std::endl; return; }

		if (m_sceneMap.find(scene->Name) != m_sceneMap.end()) {
			std::cerr << "Scene with name '" << scene->Name << "' already exists!" << std::endl;
			return;
		}

		m_sceneMap[scene->Name] = scene.get();
		m_activeScenes.push_back(std::move(scene));
	}

	void Engine::RemoveScene(Scene* scene) {
		if (!scene) {
			std::cerr << "Attempted to remove a null Scene!" << std::endl;
			return;
		}

		m_scenesToRemove.push_back(scene);
	}

	void Engine::Run() {
		std::cout << "SLAM Application Started!" << std::endl;

		while (!WindowShouldClose()) {
			float dt = GetFrameTime();
			if (m_activeScenes.size() > 0) {
				for (auto& scene : m_activeScenes) {
					if (scene) {
						scene->_update(dt);
						scene->_render();
					}
				}
				BeginDrawing();
				ClearBackground(RAYWHITE);
				for (auto& scene : m_activeScenes) {
					if (scene) {
						scene->_draw();
					}
				}
				EndDrawing();
				cleanupRemovedScenes();
			}
			else {
				std::cerr << "No active scenes to render!" << std::endl;
			}
		}
		CloseWindow();
	}

	void Engine::cleanupRemovedScenes() {
		for (Scene* toRemove : m_scenesToRemove) {
			for (auto it = m_activeScenes.begin(); it != m_activeScenes.end(); ++it) {
				if (it->get() == toRemove) {
					m_sceneMap.erase(it->get()->Name);
					m_activeScenes.erase(it);
					break;
				}
			}
		}
		m_scenesToRemove.clear();
	}
}
