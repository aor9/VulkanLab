#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace aor 
{

	class AoraWindow
	{
	public:
		AoraWindow(int w, int h, std::string name);
		~AoraWindow();

		AoraWindow(const AoraWindow&) = delete;
		AoraWindow&operator = (const AoraWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool wasWindowResized() { return bFrameBufferResized; }
		void resetWindowResizedFlag() { bFrameBufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return window; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		int width;
		int height;
		bool bFrameBufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}