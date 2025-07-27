#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "Scene.h"
#include "raylib.h"

namespace slam {
	class Engine
	{
	public:
		explicit Engine(const int windowWidth = 800, const int windowHeight = 600, const char* windowTitle = "SLAM")
			: m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_windowTitle(windowTitle) {
			InitWindow(m_windowWidth, m_windowHeight, m_windowTitle);
			SetTargetFPS(60);
		}

		void SetScene(std::unique_ptr<Scene> scene);
		void AddScene(std::unique_ptr<Scene> scene);
		void RemoveScene(Scene *scene);
		void Run();

	private:
		int m_windowWidth, m_windowHeight;
		const char* m_windowTitle;
		std::vector<std::unique_ptr<Scene>> m_activeScenes;
		std::unordered_map<std::string, Scene*> m_sceneMap;
		std::vector<Scene*> m_scenesToRemove;

		void _stageScene(std::unique_ptr<Scene> scene);
		void _cleanupRemovedScenes();
	};

};

