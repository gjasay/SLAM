#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <raylib.h>
#include <string>
#include <iostream>
#include <type_traits>

namespace slam {
	class FPCamera;
	class GameObject;
	class Engine;

	class Scene
	{
	public:
		Scene(std::string name, std::unique_ptr<FPCamera> camera = nullptr, int width = 800, int height = 600);
		~Scene();

		std::string Name;
		Vector2 Position = { 0, 0 };

		// Internal methods
		void _render();
		void _draw();
		void _update(float dt);
		Camera3D* _getActiveCamera();
		void _setEngine(Engine* eng) { m_engine = eng; }

		// Scene lifecycle methods
		virtual void OnEnter() {}
		virtual void OnExit() {}
		virtual void OnUpdate(float dt) {}

		void AddGameObject(std::unique_ptr<GameObject> obj);
		void DestroyGameObject(GameObject* obj) {}  // TODO: Implement proper destruction logic

		template <typename T>
		T* GetGameObjectByName(std::string name) {
			static_assert(std::is_base_of<GameObject, T>::value, "T must be derived from GameObject");

			auto it = m_gameObjectMap.find(name);
			if (it != m_gameObjectMap.end()) {
				return dynamic_cast<T*>(it->second);
			}

			std::cerr << "GameObject with name '" << name << "' not found in the scene!" << std::endl;
			return nullptr;
		}

		void StartScene(std::unique_ptr<Scene> scene);
		void LoadScene(std::unique_ptr<Scene> scene);
		void Destroy();

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
		std::unordered_map<std::string, GameObject*> m_gameObjectMap;
		std::unique_ptr<FPCamera> m_camera;
		Engine* m_engine = nullptr;
		RenderTexture m_renderTexture;

		void render3D();
		void render2D();
	};
}
