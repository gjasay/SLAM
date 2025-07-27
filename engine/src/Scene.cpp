#include "../include/Scene.h"
#include "../include/Camera.h"
#include "../include/Engine.h"
#include <iostream>
#include <string>
#include <raylib.h>

namespace slam
{
	Scene::Scene(const std::string &name, const int width, const int height)
		: name(name), m_renderTexture(LoadRenderTexture(width, height)) {
		auto camera = this->CreateEntity("Camera");
		auto camCtrl = _ecs.AddComponent<PlayerCameraController>(camera);
		camCtrl->entity = &camera;
		m_active3DCamera = camCtrl->GetCamera();
	}

	Scene::~Scene() {
		UnloadRenderTexture(m_renderTexture);
		std::cout << "Scene destroyed!" << std::endl;
	}

	void Scene::_enter() const {
		const auto entities = _ecs.GetAllEntities();

		for (const auto& entity : entities) {
			if (_ecs.HasComponent<ScriptComponent>(entity)) {
				const auto& scriptComponent = _ecs.GetComponent<ScriptComponent>(entity);
				scriptComponent.OnEnter();
			}
		}
	}

	void Scene::_update(const float dt) const {
		const auto scriptComponents = _ecs.GetAllComponents<ScriptComponent>();

		for (const auto* scriptComponent : scriptComponents) {
			scriptComponent->Update(dt);
		}
	}

	void Scene::_render() const {
		if (m_active3DCamera) {
			BeginTextureMode(m_renderTexture);
			ClearBackground(RAYWHITE);
			BeginMode3D(*m_active3DCamera);
			_render3D();
			EndMode3D();
			_render2D();
			EndTextureMode();
		}
		else {
			BeginTextureMode(m_renderTexture);
			ClearBackground(RAYWHITE);
			_render2D();
			EndTextureMode();
		}
	}

	void Scene::_draw() const {
		DrawTextureRec(m_renderTexture.texture,
			{ 0, 0, static_cast<float>(m_renderTexture.texture.width), static_cast<float>(-m_renderTexture.texture.height) },
			position, WHITE);
	}

	void Scene::_render3D() const {
		if (m_active3DCamera) {
			RenderMeshes(_ecs);
		}
	}

	void Scene::_render2D() const {
	  canvas.Draw();
	}

	Entity Scene::CreateEntity(const std::string &name) {
		Entity entity = _ecs.CreateEntity(name);
		 _ecs.AddComponent<Transform>(entity);
		return entity;
	}

	void Scene::StartScene(std::unique_ptr<Scene> scene) const {
		if (m_engine) {
			m_engine->SetScene(std::move(scene));
		}
	}

	void Scene::LoadScene(std::unique_ptr<Scene> scene) const {
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