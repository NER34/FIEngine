#include "ResourceManager/ResourceManager.h"

#include <ranges>

#include "ResourceManager/ResourceContainer.h"
#include "ResourceManager/Resources/MaterialResource.h"

ResourceManager::~ResourceManager()
{
    m_registeredContainers.clear();
}

ResourceManager& ResourceManager::Get()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::FreeAllResources()
{
    for (auto& container : m_registeredContainers | std::views::values)
    {
        container->FreeAllResources();
    }
}
