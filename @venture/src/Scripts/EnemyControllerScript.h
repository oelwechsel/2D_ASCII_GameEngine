#pragma once
#include <Flux.h>
#include "MapRendererScript.h"
#include "NPCWindowScript.h"
#include "ConsoleManagerScript.h"


class EnemyControllerScript : public Flux::IScript
{
    static EnemyControllerScript* s_Instance;

    //-------------------------------------------//
    //----------------Variables------------------//
    //-------------------------------------------//

private:
    // define your private Variables here

public:
    // define your public Variables here

    enum class EnemyState { Neutral, Charging };
    EnemyState e_BossEnemyFightState;

    int m_BossEnemyHP = 4;
    int m_BossEnemyPhase = 1;

    //-------------------------------------------//
    //----------------Functions------------------//
    //-------------------------------------------//

public:
    // define your public Functions here

    static EnemyControllerScript* Get()
    {
        return s_Instance;
    }

    static EnemyControllerScript& Instance()
    {
        return *s_Instance;
    }

private:
    // define your private Functions here

    //---------------------------------------------------------------//
    //----------------predefined override functions------------------//
    //---------------------------------------------------------------//

    void Start() override
    {
        if (s_Instance && s_Instance != this)
        {
            // Es gibt bereits eine gültige Instanz – wir ignorieren diese neue
            FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;
    }

    void Update(float deltaTime) override
    {
        
    }


    void OnImGuiRender() override
    {

    }

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }

};

inline EnemyControllerScript* EnemyControllerScript::s_Instance = nullptr;

// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(EnemyControllerScript);
