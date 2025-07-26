#pragma once
#include <memory>
#include <vector>
#include "Entity.h"

namespace slam {
    class Scene;

    // Base class for scripts that can be attached to entities in the scene
    class Script {
    public:
        Entity* entity = nullptr;
        Scene* scene = nullptr;

        Script(Entity* entity, Scene* scene) : entity(entity), scene(scene) {}

        virtual ~Script();

        virtual void OnEnter() {};
        virtual void Update(float dt) {};
        virtual void OnExit() {};
    };

    // Component that holds a script instance for an entity
    struct ScriptComponent {
        std::vector<std::unique_ptr<slam::Script>> Scripts;

        void OnEnter() const {
            for (auto& script : Scripts) {
                script->OnEnter();
            }
        }

        void Update(const float dt) const {
            for (auto& script : Scripts) {
                script->Update(dt);
            }
        }

        void OnExit() const {
            for (auto& script : Scripts) {
                script->OnExit();
            }
        }
    };
}
