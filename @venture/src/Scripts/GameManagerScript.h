#pragma once
#include <Flux.h>
#include "internal/Entity.h"

class GameManagerScript : public Flux::IScript
{
public:
    static GameManagerScript* s_Instance;

    GameManagerScript() = default;

    std::vector<Entity> entities;

    bool m_playerIsInteracting = false;

public:

    static GameManagerScript* Get()
    {
        return s_Instance;
    }

    static GameManagerScript& Instance()
    {
        return *s_Instance;
    }

    void Start() override
    {
        s_Instance = this;

        Entity player(14, 14, '@', "@", true);
        entities.push_back(player);

        Entity questionNPC(33, 32, '?', "?", false);
        entities.push_back(questionNPC);
    }

    void Update(float deltaTime) override {}

    void OnDestroy() override
    {
        s_Instance = nullptr;
    }
};

GameManagerScript* GameManagerScript::s_Instance = nullptr;

REGISTER_SCRIPT(GameManagerScript);
