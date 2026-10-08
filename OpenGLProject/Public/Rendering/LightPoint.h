#pragma once
#include "glm/vec3.hpp"

class LightPoint
{
public:
    
    glm::vec3 getPosition();
    glm::vec3 getColor();
    
    void setPosition(glm::vec3 newPos);
    void setColor(glm::vec3 newColor);
    
private:
    
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    
};
