#pragma once

#include "AoraWindow.h"
#include "AoraPipeline.h"
#include "AoraDevice.h"
#include "AoraSwapchain.h"
#include "AoraModel.h"
#include "AoraGameObject.h"

// std
#include <memory>
#include <vector>

namespace aor
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator = (const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		AoraWindow aoraWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		AoraDevice aoraDevice{ aoraWindow };
		std::unique_ptr<AoraSwapchain> aoraSwapchain;
		std::unique_ptr<AoraPipeline> aoraPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<AoraGameObject> gameObjects;
	};
}