#include "Rendering/LightPoint.h"

glm::vec3 LightPoint::getPosition()
{
    return m_position;
}

glm::vec3 LightPoint::getColor()
{
    return m_color;
}

void LightPoint::setPosition(glm::vec3 newPos)
{
    m_position = newPos;
}

void LightPoint::setColor(glm::vec3 newColor)
{
    m_color = newColor;
}
