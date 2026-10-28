#pragma once

#include "glm/glm.hpp"

#include "ComponentFactory/ComponentFactory.h"

class LightComponent
{
public:
    
    glm::vec3 m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    
    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, LightComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, LightComponent);
    
};
