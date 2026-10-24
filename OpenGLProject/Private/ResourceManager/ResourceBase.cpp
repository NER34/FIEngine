#include "ResourceManager/ResourceBase.h"

#include "Utilities/Utilities.h"


/*
void ResourceBase::SetFilePath(const std::string& filePath)
{
    m_filePath = filePath;
}
*/

/*
void ResourceBase::SetName(const std::string& name)
{
    m_name = name;
}
*/

const std::string& ResourceBase::GetFilePath() const
{
    return m_filePath;
}

std::string ResourceBase::GetFullFilePath() const
{
    return (GetResourcesPath() / m_filePath).string();
}

/*
const std::string& ResourceBase::GetName() const
{
    return m_name;
}
*/
