#pragma once
#include "raylib.h"
#include <vector>
#include <string>

namespace slam {
	struct Entity {
		size_t id;
		std::string name;
		std::vector<std::string> tags;

		Entity(size_t id, const std::string& name = "") : id(id), name(name) { }

		bool operator==(const Entity& other) const {
			return id == other.id;
		}
		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}
	};

	struct Transform {
		Vector3 Position = {0.0f, 0.0f, 0.0f};
		Vector3 Rotation = {0.0f, 0.0f, 0.0f};
		Vector3 Scale = {1.0f, 1.0f, 1.0f};
	};

	struct MeshRenderer {
		::Model Mesh;
		::Color Color = ::GRAY;
		bool IsVisible = true;
		bool ShowWires = true;

		~MeshRenderer() {
			::UnloadModel(Mesh);
		}
	};
}

template <>
struct std::hash<slam::Entity> {
	size_t operator()(const slam::Entity& entity) const {
		return std::hash<size_t>()(entity.id);
	}
};

