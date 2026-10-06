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
        gameObject->endPlay();
        delete gameObject;
        m_gameObjects.erase(it);
    }
}

void GameObjectManager::destroyAllGameObjects()
{
    for (GameObject* gameObject : m_gameObjects)
    {
        gameObject->endPlay();
        delete gameObject;
    }
    m_gameObjects.clear();
}

void GameObjectManager::update(float deltaTime)
{
    for (GameObject* gameObject : m_gameObjects)
    {
        gameObject->update(deltaTime);
    }
}
