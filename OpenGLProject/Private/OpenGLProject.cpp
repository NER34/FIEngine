// OpenGLProject.cpp : Defines the entry point for the application.
//

#include "OpenGLProject.h"

#include <filesystem>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Utilities/Logger.h"

const char* vertexShaderFilePath = "Shaders/VertexShader.vert";
const char* fragmentShaderFilePath = "Shaders/FragmentShader.frag";

int main()
{
	// Define vertices
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};	
	
	GLuint indices[] = {
		2, 1, 0,
		3, 2, 0
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
	{
		MaterialDesc materialDesc;
		materialDesc.m_shadersDesc = {
			{ .m_shaderType = GL_VERTEX_SHADER, .m_shaderFilePath = vertexShaderFilePath },
			{ .m_shaderType = GL_FRAGMENT_SHADER, .m_shaderFilePath = fragmentShaderFilePath }
		};
		Material material(materialDesc);
	
		MeshDesc meshDesc;
		meshDesc.m_vertices.assign(vertices, vertices + sizeof(vertices) / sizeof(GLfloat));
		meshDesc.m_indices.assign(indices, indices + sizeof(indices) / sizeof(GLuint));
		Mesh mesh(meshDesc);
	
		glViewport(0, 0, 640, 480);
	
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		
			material.activate();
			mesh.draw();		

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
