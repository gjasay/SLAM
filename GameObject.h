#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Scene.h"
#include <string>

namespace slam {

	// Base class for GameObject
	class GameObject
	{
	public:
	GameObject(std::string name, Vector3 pos, Vector3 rot) : Name(name), Position(pos), Rotation(rot) {}
	virtual ~GameObject() = default;

	std::string Name;
	std::string Tag;

	Vector3 Position;
	Vector3 Rotation;

	virtual void _Draw() {}
	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual void Update(float dt) {}
	};

	class Model : public GameObject 
	{
	public:
		Model(::Model mesh, std::string name, Vector3 pos = {0,0,0}, Vector3 rot = {0,1,0}, Color color = ::BLACK)
			: Mesh(mesh), Color(color), GameObject(name, pos, rot) {}
		
		Color Color;
		Vector3 Scale = { 1, 1, 1 };
		::Model Mesh;

		bool IsVisible = true;
		bool ShowWires = true;


		virtual void _Draw() override {
			if (!IsVisible) return;
			auto RotationRadians = Vector3{ Rotation.x * DEG2RAD, Rotation.y * DEG2RAD, Rotation.z * DEG2RAD };
			Mesh.transform = MatrixRotateXYZ(RotationRadians);
			DrawModelEx(Mesh, Position, { 0, 1, 0 }, 0, Scale, Color);
			ShowWires ? DrawModelWiresEx(Mesh, Position, { 0, 1, 0 }, 0, Scale, ::PINK) : void();
		}
	
	};

	class Cube : public Model {
	public:
		Cube(std::string name, Vector3 pos, float size, ::Color color)
			: Model(LoadModelFromMesh(GenMeshCube(size, size, size)), name, pos, { 0, 0, 0 }, color) {
		}
	};

}
