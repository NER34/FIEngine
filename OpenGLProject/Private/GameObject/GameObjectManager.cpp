#include "GameObject/GameObjectManager.h"

GameObjectManager& GameObjectManager::get()
{
    static GameObjectManager instance;
    return instance;
}

GameObjectManager::~GameObjectManager()
{
    destroyAllGameObjects();
}

void GameObjectManager::destroyGameObject(GameObject* gameObject)
{
    auto it = m_gameObjects.find(gameObject);
    if (it != m_gameObjects.end())
    {
        gameObject->EndPlay();
        delete gameObject;
        m_gameObjects.erase(it);
    }
}

void GameObjectManager::destroyAllGameObjects()
{
    for (GameObject* gameObject : m_gameObjects)
    {
        gameObject->EndPlay();
        delete gameObject;
    }
    m_gameObjects.clear();
}

void GameObjectManager::Update(float deltaTime)
{
    for (GameObject* gameObject : m_gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}
