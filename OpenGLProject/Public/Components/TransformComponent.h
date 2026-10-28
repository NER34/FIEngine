#pragma once

#include "Utilities/Utilities.h"
#include "ComponentFactory/ComponentFactory.h"

class TransformComponent
{
public:
    
    Transform m_transform;
    bool m_bModified = false;
    
    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, TransformComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, TransformComponent);
    
};
