#include "first_app.h"

#include "KeyboardMovementController.h"
#include "AoraCamera.h"
#include "SimpleRenderSystem.h"
#include "AoraBuffer.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

//std
#include <stdexcept>
#include <array>
#include <chrono>
#include <cassert>
#include <numeric>

namespace aor
{
	struct GlobalUbo
	{
		glm::mat4 projectionView{ 1.f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.f, -3.f, -1.f });
	};

	FirstApp::FirstApp() { loadGameObjects(); }

	FirstApp::~FirstApp() {}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<AoraBuffer>> uboBuffers(AoraSwapchain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<AoraBuffer>(
				aoraDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
			uboBuffers[i]->map();
		}

		SimpleRenderSystem simpleRenderSystem{ aoraDevice, aoraRenderer.getSwapChainRenderPass() };
        AoraCamera camera{};

        auto viewerObject = AoraGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!aoraWindow.shouldClose())
		{
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(aoraWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = aoraRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
			
			if (auto commandBuffer = aoraRenderer.beginFrame())
			{
				int frameIndex = aoraRenderer.getFrameIndex();
				FrameInfo frameInfo{frameIndex,	frameTime, commandBuffer, camera};

				// update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				aoraRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				aoraRenderer.endSwapChainRenderPass(commandBuffer);
				aoraRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(aoraDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<AoraModel> aoraModel = AoraModel::createModelFromFile(aoraDevice, "models/flat_vase.obj");

        auto flatVase = AoraGameObject::createGameObject();
		flatVase.model = aoraModel;
		flatVase.transform.translation = { -.5f, .5f, 2.5f };
		flatVase.transform.scale = glm::vec3(3.f, 1.5f, 3.f);
        gameObjects.push_back(std::move(flatVase));

		aoraModel = AoraModel::createModelFromFile(aoraDevice, "models/smooth_vase.obj");
		auto smoothVase = AoraGameObject::createGameObject();
		smoothVase.model = aoraModel;
		smoothVase.transform.translation = { .5f, .5f, 2.5f };
		smoothVase.transform.scale = glm::vec3(3.f, 1.5f, 3.f);
		gameObjects.push_back(std::move(smoothVase));
	}
}