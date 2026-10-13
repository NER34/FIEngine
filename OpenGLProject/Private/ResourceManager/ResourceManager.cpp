#include "ResourceManager/ResourceManager.h"

#include <ranges>

#include "ResourceManager/ResourceContainer.h"
#include "ResourceManager/Resources/MeshResource.h"

ResourceManager::~ResourceManager()
{
    for (auto& container : m_registeredContainers | std::views::values)
    {
        delete container;
    }
    m_registeredContainers.clear();
}

ResourceManager& ResourceManager::Get()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::Initialize()
{
    RegisterResourceType<ResourceContainer<MeshResource>, MeshResource>();
}

void ResourceManager::FreeAllResources()
{
    for (auto& container : m_registeredContainers | std::views::values)
    {
        container->FreeAllResources();
    }
}

/*
void ResourceManager::LoadResource_Internal(
    const std::string& name, const std::string& fileName, IResourceContainer* container
    )
{
    container->AllocateResource(name, fileName);
}

void ResourceManager::FreeResource_Internal(const std::string& name, IResourceContainer* container)
{
    container->FreeResource(name);
}
*/
