#pragma once

#include <cassert>
#include <typeindex>

#include "Registry/Registry.h"
#include "Utilities/Logger.h"

class ISystemBase
{
public:

    virtual ~ISystemBase() = default;

    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Deinitialize() {}

};

class SystemLocator : public Registry<std::type_index, ISystemBase>
{
public:

    static SystemLocator& Get()
    {
        static SystemLocator instance;
        return instance;
    }

    template <typename TSystem, typename ... TArgs>
    bool RegisterSystem(TArgs&& ... args)
    {
        return RegisterSystem<TSystem, TSystem>(std::forward<TArgs>(args)...);
    }

    template <typename TSystemID, typename TSystem, typename ... TArgs>
    bool RegisterSystem(TArgs&& ... args)
    {
        if (!RegisterObject<TSystem>(typeid(TSystemID), std::forward<TArgs>(args)...))
        {
            LOG_ERROR("SystemLocator: system {} is already registered", typeid(TSystemID).name());
            return false;
        }
        return true;
    }

    // Logs an error and returns nullptr when the system is not registered
    template <typename TSystemID>
    ISystemBase* GetSystem() const
    {
        ISystemBase* system = GetObject(typeid(TSystemID));
        if (!system)
        {
            LOG_ERROR("SystemLocator: system {} is not registered", typeid(TSystemID).name());
        }
        return system;
    }

    template <typename ... TSystemIDs>
    void InitializeSystems() const
    {
        (InitializeSystem<TSystemIDs>(), ...);
    }

    // Missing systems are skipped without logging to avoid per-frame spam;
    // they are already reported by InitializeSystems
    template <typename ... TSystemIDs>
    void UpdateSystems(float deltaTime) const
    {
        (UpdateSystem<TSystemIDs>(deltaTime), ...);
    }

    template <typename ... TSystemIDs>
    void DeinitializeSystems() const
    {
        (DeinitializeSystem<TSystemIDs>(), ...);
    }

private:

    template <typename TSystemID>
    void InitializeSystem() const
    {
        if (ISystemBase* system = GetSystem<TSystemID>())
        {
            system->Initialize();
        }
    }

    template <typename TSystemID>
    void UpdateSystem(float deltaTime) const
    {
        if (ISystemBase* system = GetObject(typeid(TSystemID)))
        {
            system->Update(deltaTime);
        }
    }

    template <typename TSystemID>
    void DeinitializeSystem() const
    {
        if (ISystemBase* system = GetSystem<TSystemID>())
        {
            system->Deinitialize();
        }
    }

};
