#pragma once
#include <set>

#include "GameObject.h"

class GameObject;

class GameObjectManager
{
public:
    
    static GameObjectManager& get();
    
    GameObjectManager() = default;
    ~GameObjectManager();
    
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<GameObject, T>>>
    T* createGameObject()
    {
        T* gameObject = new T();
        gameObject->beginPlay();
        m_gameObjects.insert(gameObject);
        return gameObject;
    }
    
    void destroyGameObject(GameObject* gameObject);
    void destroyAllGameObjects();
    
    void update(float deltaTime);

private:
    
    std::set<GameObject*> m_gameObjects;
    
};
