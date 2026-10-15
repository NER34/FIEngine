#pragma once

#include <memory>
#include <string>

class ResourceBase
{
public:
    
    ResourceBase() = default;
    virtual ~ResourceBase() = default;
    
    //void SetFilePath(const std::string& filePath);
    //void SetName(const std::string& name);
    
    virtual void Reload() {}
    virtual void Free() {}
    
    const std::string& GetFilePath() const;
    std::string GetFullFilePath() const;
    //const std::string& GetName() const;
    
private:
    
    template <typename T>
    friend class ResourceContainer;
    
    //std::string m_name;
    std::string m_filePath;
    std::weak_ptr<ResourceBase> m_this;
    
};
