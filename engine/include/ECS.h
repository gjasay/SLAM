#pragma once
#include "../include/Entity.h"
#include "../include/Script.h"
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

namespace slam {
  class ECS {
  public:
    explicit ECS(Scene* scene) : m_scene(scene) {}

    Entity createEntity(const std::string &name = "") {
      Entity entity(nextId++, name);
      entities.emplace_back(entity);
      return entity;
    }

    template<typename Component>
    const Component &GetComponent(const Entity &entity) const {
      if constexpr (std::is_base_of_v<Script, Component>) {
        const auto &[Scripts] = scripts.at(entity);
        for (const auto &script: Scripts) {
          if (const auto *specificScript = dynamic_cast<const Component *>(script.get())) {
            return *specificScript;
          }
        }
        throw std::runtime_error("Script component not found for the given entity.");
      } else {
        const auto &componentMap = getComponentMap<Component>();
        auto it = componentMap.find(entity);
        if (it == componentMap.end()) {
          throw std::runtime_error("Component not found for the given entity.");
        }
        return it->second;
      }
    }

    template<typename Component>
    Component* GetComponent(const Entity &entity) {
      if constexpr (std::is_base_of_v<Script, Component>) {
        auto &[Scripts] = scripts.at(entity);
        for (auto &script: Scripts) {
          if (auto *specificScript = dynamic_cast<Component *>(script.get())) {
            return *specificScript;
          }
        }
        throw std::runtime_error("Script component not found for the given entity.");
      } else {
        auto &componentMap = getComponentMap<Component>();
        auto it = componentMap.find(entity);
        if (it == componentMap.end()) {
          throw std::runtime_error("Component not found for the given entity.");
        }
        return &it->second;
      }
    }

    template<typename Component>
    Component* AddComponent(Entity &entity) {
      if constexpr (std::is_base_of_v<Script, Component>) {
        auto &[Scripts] = scripts[entity];
        Scripts.emplace_back(std::make_unique<Component>(&entity, m_scene));
        Scripts.back()->entity = &entity;
        return static_cast<Component *>(Scripts.back().get());
      } else {
        auto &componentMap = getComponentMap<Component>();
        componentMap[entity] = Component();
        return &componentMap[entity];
      }
    }

    template<typename Component>
    bool HasComponent(const Entity &entity) const {
      if constexpr (std::is_base_of_v<Script, Component>) {
        const auto &[Scripts] = scripts.at(entity);
        return std::any_of(Scripts.begin(), Scripts.end(),
                           [](const std::unique_ptr<Script> &script) {
                             return dynamic_cast<Component *>(script.get()) != nullptr;
                           });
      } else {
        const auto &componentMap = getComponentMap<Component>();
        return componentMap.find(entity) != componentMap.end();
      }
    }

    template<typename Component>
    void RemoveComponent(const Entity &entity) {
      if constexpr (std::is_base_of_v<Script, Component>) {
        auto &[Scripts] = scripts[entity];
        Scripts.erase(
          std::remove_if(Scripts.begin(), Scripts.end(),
                         [](const std::unique_ptr<Script> &script) {
                           return dynamic_cast<Component *>(script.get()) != nullptr;
                         }),
          Scripts.end());
      } else {
        auto &componentMap = getComponentMap<Component>();
        componentMap.erase(entity);
      }
    }

    template<typename Component>
    std::vector<Component *> GetAllComponents() const {
      std::vector<Component *> components;
      if constexpr (std::is_base_of_v<Script, Component>) {
        for (auto &[entity, scriptComponent]: scripts) {
          for (auto &script: scriptComponent.Scripts) {
            if (auto *specificScript = dynamic_cast<Component *>(script.get())) {
              components.push_back(specificScript);
            }
          }
        }
      } else {
        const auto &componentMap = getComponentMap<Component>();
        for (const auto &[entity, component]: componentMap) {
          components.push_back(const_cast<Component *>(&component));
        }
      }
      return components;
    }

    std::vector<Entity> GetAllEntities() const {
      return entities;
    }

  private:
    Scene* m_scene;

    template<typename Component>
    const std::unordered_map<Entity, Component> &getComponentMap() const {
      if constexpr (std::is_same_v<Component, Transform>) {
        return transforms;
      } else if constexpr (std::is_same_v<Component, MeshRenderer>) {
        return meshRenderers;
      } else if constexpr (std::is_same_v<Component, ScriptComponent>) {
        return scripts;
      } else {
        static_assert(false, "Unsupported component type.");
      }
    }

    template<typename Component>
    std::unordered_map<Entity, Component> &getComponentMap() {
      if constexpr (std::is_same_v<Component, Transform>) {
        return transforms;
      } else if constexpr (std::is_same_v<Component, MeshRenderer>) {
        return meshRenderers;
      } else if constexpr (std::is_same_v<Component, ScriptComponent>) {
        return scripts;
      } else {
        static_assert(false, "Unsupported component type.");
      }
    }

    std::unordered_map<Entity, Transform> transforms;
    std::unordered_map<Entity, MeshRenderer> meshRenderers;
    std::unordered_map<Entity, ScriptComponent> scripts;
    std::vector<Entity> entities;
    size_t nextId = 1;
  };

  void RenderMeshes(const ECS &ecs);
}
