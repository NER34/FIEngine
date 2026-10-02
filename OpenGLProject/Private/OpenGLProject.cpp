// OpenGLProject.cpp : Defines the entry point for the application.
//

#include "OpenGLProject.h"

#include <filesystem>
#include <algorithm>

#include "glad/glad.h"

#include "InputSystem/InputContext.h"
#include "InputSystem/InputManager.h"
#include "Rendering/Camera.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"
#include "Window/GLFW/WindowGLFW.h"

const char* vertShaderPath = "Shaders/VertexShader.vert";
const char* fragShaderPath = "Shaders/FragmentShader.frag";

int main()
{
	// Define vertices
	GLfloat vertices[] = {
		0.0f,  0.5f,  0.5f,		0.0f, 0.0f, // top left
		0.0f, -0.5f,  0.5f,		1.0f, 0.0f, // top right
		0.0f,  0.5f, -0.5f,		0.0f, 1.0f, // bot left
		0.0f, -0.5f, -0.5f,		1.0f, 1.0f, // bot right
	};	
	
	GLuint indices[] = {
		2, 1, 0,
		2, 3, 1
	};
	
	FIE::WindowDesc windowDesc;
	windowDesc.m_title = "OpenGL Project";
	FIE::WindowGLFW window(windowDesc);
	
	CameraDesc cameraDesc;
	cameraDesc.m_aspectRatio = (float)window.getWindowDesc().m_width / (float)window.getWindowDesc().m_height;
		
	Camera camera(cameraDesc);
	camera.setPosition({ 1.0f, 0.0f, 0.0f });
	camera.setRotation({ 0.0f, 0.0f, 180.0f });
	
	FIE::IInputManager& inputManager = FIE::IInputManager::Get();
	FIE::InputContext inputContext;
	inputManager.setInputContext(&inputContext);
	
	float cameraMovSpeed = 1.0f;
	struct CameraMovDir
	{
		int m_forwardMult = 0;
		int m_rightMult = 0;
		int m_upMult = 0;
	} cameraMovDir;
	
#define CHANGE_CAMERA_POS(dir, multi) \
	[&cameraMovDir](FIE::InputIDs inputID, float inputValue) \
	{ \
		if (inputValue > 0.5f) \
		{ \
			cameraMovDir.dir += multi; \
		} \
		else \
		{ \
			cameraMovDir.dir -= multi; \
		} \
	}
	
	inputContext.assignHandle(FIE::KEY_W, CHANGE_CAMERA_POS(m_forwardMult, 1));
	inputContext.assignHandle(FIE::KEY_S, CHANGE_CAMERA_POS(m_forwardMult, -1));
	inputContext.assignHandle(FIE::KEY_D, CHANGE_CAMERA_POS(m_rightMult, 1));
	inputContext.assignHandle(FIE::KEY_A, CHANGE_CAMERA_POS(m_rightMult, -1));
	inputContext.assignHandle(FIE::KEY_E, CHANGE_CAMERA_POS(m_upMult, 1));
	inputContext.assignHandle(FIE::KEY_Q, CHANGE_CAMERA_POS(m_upMult, -1));

	float cameraRotSpeed = 1.0f;
	struct CameraRot
	{
		float m_pitch = 0;
		float m_yaw = 0;
	} cameraRot;

#define CHANGE_CAMERA_ROT(axis) \
	[&cameraRot, cameraRotSpeed](FIE::InputIDs inputID, float inputValue) \
	{ \
		cameraRot.axis += inputValue * cameraRotSpeed; \
	}
	
	inputContext.assignHandle(FIE::MOUSE_MOVE_X, CHANGE_CAMERA_ROT(m_yaw));
	inputContext.assignHandle(FIE::MOUSE_MOVE_Y, CHANGE_CAMERA_ROT(m_pitch));
	
	inputContext.assignHandle(FIE::KEY_ESCAPE, [&window](FIE::InputIDs, float)
	{
		window.requestWindowClose();
	});
	
	// Load GLAD and init OpenGL
	gladLoadGL();
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	{
		auto resourcesPath = getResourcesPath();
		
		MaterialDesc materialDesc;
		materialDesc.m_shadersDesc = {
			{ GL_VERTEX_SHADER, (resourcesPath / vertShaderPath).string() },
			{ GL_FRAGMENT_SHADER, (resourcesPath / fragShaderPath).string() }
		};
		materialDesc.m_texturesDesc = {
			{ "TexSampler", { (resourcesPath / "Textures/test_image.png").string() } }
		};
		Material material(materialDesc);
		
		MeshDesc meshDesc;
		meshDesc.m_vertices.assign(vertices, vertices + sizeof(vertices) / sizeof(GLfloat));
		meshDesc.m_indices.assign(indices, indices + sizeof(indices) / sizeof(GLuint));
		Mesh mesh(meshDesc);
		
		MeshSpawnData meshSpawnData(mesh);
		meshSpawnData.m_transform.m_position = glm::vec3(2.0f, 0.0f, 0.0f);
		meshSpawnData.m_transform.m_rotation = glm::vec3(0.0f, 0.0f, 180.0f);
	
		glViewport(
			0, 0,
			(GLsizei)window.getWindowDesc().m_width,
			(GLsizei)window.getWindowDesc().m_height
			);
	
		auto prevTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		
		float globalVal = 0.0f;
		
		while (!window.isWindowShouldClose())
		{
			prevTime = currTime;
			currTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currTime - prevTime).count();
			
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);\
			
			inputManager.update();
			
			auto cameraPos = camera.getPosition();
			auto cameraForward =  camera.getTransform().getForwardVector();
			auto cameraRight = camera.getTransform().getRightVector();
			cameraPos += cameraForward * cameraMovSpeed * deltaTime * (float)cameraMovDir.m_forwardMult;
			cameraPos += cameraRight * cameraMovSpeed * deltaTime * (float)cameraMovDir.m_rightMult;
			cameraPos += glm::vec3(0.0f, 0.0f, 1.0f) * cameraMovSpeed * deltaTime * (float)cameraMovDir.m_upMult;
			camera.setPosition(cameraPos);
			
			cameraRot.m_pitch = std::clamp(cameraRot.m_pitch, -89.0f, 89.0f);
			camera.setRotation({ 0.0f, cameraRot.m_pitch, cameraRot.m_yaw });
			
			{
				globalVal += deltaTime;
			
				auto& rotation = meshSpawnData.m_transform.m_rotation;
				rotation.x = 10 * globalVal;
			
				//auto& scale = meshSpawnData.m_transform.m_scale;
				//scale = glm::vec3(1.0f) * (sin(globalVal) / 2.0f + 1.0f);
			
				//auto& position = meshSpawnData.m_transform.m_position;
				//position.z = sin(globalVal) / 2.0f;
			}
			
			material.activate();

			glm::mat4 projMat = camera.getProjectionMatrix();
			glm::mat4 viewMat = camera.getViewMatrix();
			glm::mat4 modelMat = meshSpawnData.m_transform.getModelMatrix();
			
			material.setUniform("mvp", projMat * viewMat * toLeftHandTransformMat  * modelMat);

			meshSpawnData.m_mesh.draw();	
			material.deactivate();

			window.update();
		}
	}
	
	return 0;
}
