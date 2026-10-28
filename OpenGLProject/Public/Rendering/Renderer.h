#pragma once

#include "Camera.h"
#include "LightPoint.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MaterialResource.h"
#include "ResourceManager/Resources/MeshResource.h"

struct GeometryDrawDesc
{
    GeometryDrawDesc() = delete;
    GeometryDrawDesc(const GeometryDrawDesc& other)
        : m_mesh(other.m_mesh)
        , m_material(other.m_material)
        , m_transform(other.m_transform)
    {}
    GeometryDrawDesc(const ResourceHandle<MeshResource>& mesh, const ResourceHandle<MaterialResource>& material)
        : m_mesh(mesh)
        , m_material(material)
    {}
    GeometryDrawDesc(
        const ResourceHandle<MeshResource>& mesh, 
        const ResourceHandle<MaterialResource>& material, 
        const Transform& transform
        ) 
        : m_mesh(mesh)
        , m_material(material)
        , m_transform(transform)
    {}
    
    ResourceHandle<MeshResource> m_mesh;
    ResourceHandle<MaterialResource> m_material;
    Transform m_transform;
};

struct ViewportDesc
{
    GLsizei x = 0;
    GLsizei y = 0;
    GLsizei width = 0;
    GLsizei height = 0;
};

struct RendererDesc
{
    ViewportDesc m_viewportDesc;
    CameraDesc m_cameraDesc;
};

class Renderer
{
public:
    
    static Renderer& get();
    
    Renderer() 
        : m_frontDrawList(m_drawList_1)
        , m_backDrawList(m_drawList_2)
    {}
    
    void initialize(const RendererDesc& desc);
    void addDraw(const GeometryDrawDesc& desc);
    void render();
    
    /*
    Camera& getCamera();
    LightPoint& getLightPoint();
    */
    
private:
    
    std::vector<GeometryDrawDesc>& m_frontDrawList;
    std::vector<GeometryDrawDesc>& m_backDrawList;
    
    std::vector<GeometryDrawDesc> m_drawList_1;
    std::vector<GeometryDrawDesc> m_drawList_2;
    
    Camera m_camera;
    LightPoint m_lightPoint;
    
};
