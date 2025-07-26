#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <raylib.h>
#include <string>
#include <iostream>
#include <type_traits>

#include "Entity.h"
#include "ECS.h"

namespace slam {
	class PlayerCameraController;
	class Engine;

	class Scene
	{
	public:
		explicit Scene(const std::string &name, int width = 800, int height = 600);
		~Scene();

		std::string Name;
		Vector2 Position = { 0, 0 };
		ECS _ecs = ECS(this);

		// Internal methods TODO: make private and make Engine a friend class
		void _render();
		void _draw() const;
		void _update(float dt) const;
		void _setEngine(Engine* eng) { m_engine = eng; }

		Entity CreateEntity(const std::string &name);

		void StartScene(std::unique_ptr<Scene> scene) const;
		void LoadScene(std::unique_ptr<Scene> scene) const;
		void Destroy();

		// Forward ECS methods
		template <typename Component>
		Component* GetComponent(const Entity& entity) {
			return _ecs.GetComponent<Component>(entity);
		}

		template <typename Component>
		Component* AddComponent(Entity& entity) {
			Component* instance = _ecs.AddComponent<Component>(entity);

			if constexpr (std::is_base_of_v<Script, Component>) {
				instance._setScene(this);
			}
			return instance;
		}

		template <typename Component>
		bool HasComponent(const Entity& entity) const {
			return _ecs.HasComponent<Component>(entity);
		}

		template <typename Component>
		void RemoveComponent(const Entity& entity) {
			_ecs.RemoveComponent<Component>(entity);
		}

		template <typename Component>
		std::vector<Component*> GetAllComponents() const {
			return _ecs.GetAllComponents<Component>();
		}

		std::vector<Entity> GetAllEntities() const {
			return _ecs.GetAllEntities();
		}

	private:
		::Camera3D* m_active3DCamera;
		Engine* m_engine = nullptr;
		RenderTexture m_renderTexture;

		void render3D() const;
		void render2D();
	};
}
