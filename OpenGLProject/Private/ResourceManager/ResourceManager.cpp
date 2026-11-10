#include "ResourceManager/ResourceManager.h"

#include <ranges>

#include "ResourceManager/ResourceContainer.h"
#include "ResourceManager/Resources/MaterialResource.h"

ResourceManager::~ResourceManager()
{
    m_registeredContainers.clear();
}

void ResourceManager::Deinitialize()
{
    FreeAllResources();
}

ResourceManager& ResourceManager::Get()
{
    auto* system = SystemLocator::Get().GetSystem<ResourceManager>();
    assert(system && "ResourceManager is not registered");
    return *static_cast<ResourceManager*>(system);
}

void ResourceManager::FreeAllResources()
{
    for (auto& container : m_registeredContainers | std::views::values)
    {
        container->FreeAllResources();
    }
}
