#pragma once
#include <Flux.h>

namespace fs = std::filesystem;

class GameManagerVariables : public Flux::IScript
{
public:
    static GameManagerVariables* s_Instance;

    GameManagerVariables() = default;

    bool m_isInFight = false;

public:

    static GameManagerVariables* Get()
    {
        return s_Instance;
    }

    static GameManagerVariables& Instance()
    {
        return *s_Instance;
    }

    void Start() override
    {
        if (s_Instance && s_Instance != this)
        {
            // Es gibt bereits eine gültige Instanz – wir ignorieren diese neue
            FX_WARN("GameManagerVariable: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;
    }

    void Update(float deltaTime) override {}

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }
};


inline GameManagerVariables* GameManagerVariables::s_Instance = nullptr;

REGISTER_SCRIPT(GameManagerVariables);
