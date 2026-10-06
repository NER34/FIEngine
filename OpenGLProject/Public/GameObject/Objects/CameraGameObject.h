#pragma once

#include "GameObject/GameObject.h"

class CameraGameObject : public GameObject
{
public:
    
    void beginPlay() override;
    void update(float deltaTime);
    
private:
    
    float m_cameraMovSpeed = 10.0f;
    float m_cameraRotSpeed = 100.0f;
    bool m_followMouse = false;
    
    struct CameraMovDir
    {
        int m_forwardMult = 0;
        int m_rightMult = 0;
        int m_upMult = 0;
    } m_cameraMovDir;
    
    struct CameraRot
    {
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;
        
        void zero() { m_pitch = 0.0f; m_yaw = 0.0f; }
    } m_cameraRot;
    
};
