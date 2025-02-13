#include "first_app.h"

#include "KeyboardMovementController.h"
#include "AoraCamera.h"
#include "SimpleRenderSystem.h"

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

namespace aor
{
	FirstApp::FirstApp() { loadGameObjects(); }

	FirstApp::~FirstApp() {}

	void FirstApp::run()
	{
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
				// begin offscreen shadow pass
				// render shadow casting objects
				// end offscreen shadow pass

				aoraRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				aoraRenderer.endSwapChainRenderPass(commandBuffer);
				aoraRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(aoraDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<AoraModel> aoraModel = AoraModel::createModelFromFile(aoraDevice, "models/smooth_vase.obj");

        auto gameObj = AoraGameObject::createGameObject();
		gameObj.model = aoraModel;
		gameObj.transform.translation = { .0f, .0f, 2.5f };
		gameObj.transform.scale = glm::vec3(3.f);
        gameObjects.push_back(std::move(gameObj));
	}
}