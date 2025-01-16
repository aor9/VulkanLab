#include "first_app.h"

#include "SimpleRenderSystem.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

//std
#include <stdexcept>
#include <array>
#include <cassert>

namespace aor
{
	FirstApp::FirstApp() { loadGameObjects(); }

	FirstApp::~FirstApp() {}

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ aoraDevice, aoraRenderer.getSwapChainRenderPass() };

		while (!aoraWindow.shouldClose())
		{
			glfwPollEvents();
			
			if (auto commandBuffer = aoraRenderer.beginFrame())
			{
				// begin offscreen shadow pass
				// render shadow casting objects
				// end offscreen shadow pass

				aoraRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				aoraRenderer.endSwapChainRenderPass(commandBuffer);
				aoraRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(aoraDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<AoraModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		
		auto aoraModel = std::make_shared<AoraModel>(aoraDevice, vertices);

		auto triangle = AoraGameObject::createGameObject();
		triangle.model = aoraModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}