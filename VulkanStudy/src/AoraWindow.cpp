#include "AoraWindow.h"

#include <stdexcept>

namespace aor
{
	AoraWindow::AoraWindow(int w, int h, std::string name) : width(w), height(h), windowName(name)
	{
		initWindow();
	}

	AoraWindow::~AoraWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void AoraWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void AoraWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void AoraWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto aoraWindow = reinterpret_cast<AoraWindow*>(glfwGetWindowUserPointer(window));
		aoraWindow->bFrameBufferResized = true;
		aoraWindow->width = width;
		aoraWindow->height = height;
	}
}

