#pragma once

class GLFWwindow;

namespace FIE
{
	class InputAdapterGLFW
	{
	public:

		static void Initialize(GLFWwindow* window);

	private:

		static void OnKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
		static void OnMouseInput(GLFWwindow* window, int button, int action, int mods);
		static void OnScrollInput(GLFWwindow* window, double xoffset, double yoffset);

	};
}