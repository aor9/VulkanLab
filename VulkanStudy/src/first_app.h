#pragma once

#include "AoraWindow.h"
#include "AoraPipeline.h"
#include "AoraDevice.h"
#include "AoraSwapchain.h"
#include "AoraModel.h"

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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		AoraWindow aoraWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		AoraDevice aoraDevice{ aoraWindow };
		AoraSwapchain aoraSwapchain{ aoraDevice, aoraWindow.getExtent() };
		std::unique_ptr<AoraPipeline> aoraPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<AoraModel> aoraModel;
	};
}