#pragma once
#include "Flux/IScript.h"

namespace Flux
{
    class FLUX_API ScriptRegistry
    {
    public:
        using CreateScriptFn = std::function<std::unique_ptr<IScript>()>;

        static ScriptRegistry& Get()
        {
            static ScriptRegistry instance;
            return instance;
        }

        void Register(const std::string& _name, CreateScriptFn _func)
        {
            m_FactoryMap[_name] = _func;
        }

        std::unique_ptr<IScript> CreateScript(const std::string& _name)
        {
            auto it = m_FactoryMap.find(_name);
            if (it != m_FactoryMap.end())
                return it->second();
            return nullptr;
        }

        const auto& GetAllScripts() const { return m_FactoryMap; }

    private:
        std::unordered_map<std::string, CreateScriptFn> m_FactoryMap;
    };
}

#define REGISTER_SCRIPT(ClassName) \
    namespace { \
        struct ClassName##Registrar { \
            ClassName##Registrar() { \
                Flux::ScriptRegistry::Get().Register(#ClassName, []() { return std::make_unique<ClassName>(); }); \
            } \
        }; \
        static ClassName##Registrar global_##ClassName##Registrar; \
    }
