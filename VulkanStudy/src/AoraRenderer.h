#pragma once

#pragma once

#include "AoraWindow.h"
#include "AoraPipeline.h"
#include "AoraDevice.h"
#include "AoraSwapchain.h"
#include "AoraModel.h"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace aor
{
	class AoraRenderer
	{
	public:
		AoraRenderer(AoraWindow &window, AoraDevice &device);
		~AoraRenderer();

		AoraRenderer(const AoraRenderer&) = delete;
		AoraRenderer& operator = (const AoraRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return aoraSwapchain->getRenderPass(); }
		float getAspectRatio() const { return aoraSwapchain->extentAspectRatio(); }
		bool bIsFrameInProgress() const { return bIsFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(bIsFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex]; 
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		int getFrameIndex() const {
			assert(bIsFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		AoraWindow& aoraWindow;
		AoraDevice& aoraDevice;
		std::unique_ptr<AoraSwapchain> aoraSwapchain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool bIsFrameStarted{ false };
	};
}