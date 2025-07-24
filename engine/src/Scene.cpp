#include "../include/Scene.h"
#include "../include/Camera.h"
#include "../include/GameObject.h"
#include "../include/Engine.h"
#include <iostream>
#include <string>
#include <raylib.h>

namespace slam
{
	Scene::Scene(std::string name, std::unique_ptr<FPCamera> camera, int width, int height)
		: Name(name), m_camera(std::move(camera)), m_renderTexture(LoadRenderTexture(width, height)) {}

	Scene::~Scene() {
		for (auto& gameObject : m_gameObjects) {
			if (gameObject) {
				gameObject->OnExit();
			}
		}
		m_gameObjects.clear();
		m_gameObjectMap.clear();
		if (m_camera) {
			m_camera.reset();
		}
		UnloadRenderTexture(m_renderTexture);
		std::cout << "Scene destroyed!" << std::endl;
	}

	void Scene::_update(float dt) {
		this->OnUpdate(dt);

		if (m_camera) {
			Scene::m_camera->Update(dt);
		}

		for (auto& gameObject : m_gameObjects) {
			if (gameObject) {
				gameObject->Update(dt);
			}
		}
	}

	void Scene::_render() {
		if (m_camera) {
			BeginTextureMode(m_renderTexture);
			ClearBackground(RAYWHITE);
			BeginMode3D(*m_camera->GetCamera());
			render3D();
			EndMode3D();
			render2D();
			EndTextureMode();
		}
		else {
			BeginTextureMode(m_renderTexture);
			ClearBackground(RAYWHITE);
			render2D();
			EndTextureMode();
		}
	}

	void Scene::_draw() {
		DrawTextureRec(m_renderTexture.texture,
			{ 0, 0, (float)m_renderTexture.texture.width, (float)-m_renderTexture.texture.height },
			Position, WHITE);
	}

	void Scene::render3D() {
		if (m_camera) {
			for (auto& gameObject : m_gameObjects) {
				if (gameObject) {
					gameObject->_Draw();
				}
			}
		}
	}

	void Scene::render2D() {}

	Camera3D* Scene::_getActiveCamera() {
		return m_camera ? m_camera->GetCamera() : nullptr;
	}

	void Scene::AddGameObject(std::unique_ptr<GameObject> obj) {
		if (!obj) { std::cerr << "Attempted to add a null GameObject!" << std::endl; return; }

		if (m_gameObjectMap.find(obj->Name) != m_gameObjectMap.end()) {
			std::cerr << "GameObject with name '" << obj->Name << "' already exists in the scene!" << std::endl;
			return;
		}

		obj->OnEnter();
		m_gameObjectMap[obj->Name] = obj.get();
		m_gameObjects.push_back(std::move(obj));
	}

	void Scene::StartScene(std::unique_ptr<Scene> scene) {
		if (m_engine) {
			m_engine->SetScene(std::move(scene));
		}
	}

	void Scene::LoadScene(std::unique_ptr<Scene> scene) {
		if (m_engine) {
			m_engine->AddScene(std::move(scene));
		}
	}

	void Scene::Destroy() {
		if (m_engine) {
			m_engine->RemoveScene(this);
		}
	}
}