#pragma once

#include "SystemBase/SystemLocator.h"

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MaterialResource.h"
#include "ResourceManager/Resources/MeshResource.h"
#include "Window/WindowManager.h"

struct GeometryDrawDesc
{
    GeometryDrawDesc() = delete;
    GeometryDrawDesc(
        const ResourceHandle<MeshResource>& mesh, 
        const ResourceHandle<MaterialResource>& material,
        size_t subMeshIndex
        )
        : m_mesh(mesh)
        , m_material(material)
        , m_subMeshIndex(subMeshIndex)
    {}
    
    GeometryDrawDesc(
        const ResourceHandle<MeshResource>& mesh, 
        const ResourceHandle<MaterialResource>& material, 
        const glm::mat4& transform,
        size_t subMeshIndex
        ) 
        : m_mesh(mesh)
        , m_material(material)
        , m_transform(transform)
        , m_subMeshIndex(subMeshIndex)
    {}
    
    GeometryDrawDesc(const GeometryDrawDesc& other) = default;
    GeometryDrawDesc& operator=(const GeometryDrawDesc& other) = default;
    
    GeometryDrawDesc(GeometryDrawDesc&&) noexcept = default;
    GeometryDrawDesc& operator=(GeometryDrawDesc&&) noexcept = default;
    
    ResourceHandle<MeshResource> m_mesh;
    ResourceHandle<MaterialResource> m_material;
    glm::mat4 m_transform;
    size_t m_subMeshIndex = 0;
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
    //CameraDesc m_cameraDesc;
};

class Renderer : public ISystemBase
{
public:
    
    static Renderer& Get();
    
    Renderer() 
        : m_frontDrawList(m_drawList_1)
        , m_backDrawList(m_drawList_2)
    {}
    
    void Initialize() override;
    void Update(float deltaTime) override;
    void Deinitialize() override;
    
    void Initialize(const RendererDesc& desc);
    void AddDraw(const GeometryDrawDesc& desc);
    void Render();
    
private:
    
    void OnFramebufferResize(uint32_t width, uint32_t height);
    
    void CollectMeshesForDrawing();
    void SortMeshesForDrawing();
    
    std::vector<GeometryDrawDesc>& m_frontDrawList;
    std::vector<GeometryDrawDesc>& m_backDrawList;
    
    std::vector<GeometryDrawDesc> m_drawList_1;
    std::vector<GeometryDrawDesc> m_drawList_2;
    
    //Camera m_camera;
    ResourceHandle<MaterialResource> m_defaultMaterial;
    FIE::WindowManager* m_windowManager = nullptr;
    FIE::DispatcherHandle m_framebufferResizeHandle;
};
