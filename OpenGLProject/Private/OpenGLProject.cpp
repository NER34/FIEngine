// OpenGLProject.cpp : Defines the entry point for the application.
//

#include "OpenGLProject.h"

#include <iostream>
#include <filesystem>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

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
	
	
	
	glfwInit();
	
	// Define OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW that we are using CORE profile
	// So that means we only have the modern function
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create window
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	// Set created window into current context
	glfwMakeContextCurrent(window);
	
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
	
		glViewport(0, 0, 640, 480);
	
		auto prevTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		
		float globalVal = 0.0f;
		
		//mesh.setRotation({45, 45, 45});
		while (!glfwWindowShouldClose(window))
		{
			prevTime = currTime;
			currTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currTime - prevTime).count();
			
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		
			globalVal += deltaTime;
			
			//auto rotation = mesh.getRotation();
			//rotation.z = 10*globalVal;
			//mesh.setRotation(rotation);
			
			//auto scale = mesh.getScale();
			//scale = glm::vec3(1.0f) * (sin(globalVal) / 2.0f + 1.0f);
			//mesh.setScale(scale);
			
			//auto position = mesh.getPosition();
			//position.z = sin(globalVal) / 2.0f;
			//mesh.setPosition(position);
			
			material.activate();
			
			glm::mat4 projectionMat = glm::perspective(glm::radians(90.0f), 640.0f/ 480.0f, 0.1f, 100.0f );
			float xPos = 1;
			glm::mat4 viewMat = glm::lookAt(
				toOGLCoordSys(glm::vec3(xPos, 0.0f, 0.0f)), 
				toOGLCoordSys(glm::vec3(xPos-1.0f, 0.0f, 0.0f)), 
				toOGLCoordSys(glm::vec3(0.0f, 0.0f, 1.0f))
				);
			const glm::mat4& modelMat = mesh.getModelMatrix();
			
			material.setUniform("mvp", projectionMat * viewMat * modelMat);
			/*
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			*/
			
			mesh.draw();	
			material.deactivate();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
