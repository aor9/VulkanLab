#include "AoraRenderer.h"


//std
#include <stdexcept>
#include <array>
#include <cassert>

namespace aor
{
	AoraRenderer::AoraRenderer(AoraWindow& window, AoraDevice& device) : aoraWindow(window), aoraDevice(device)
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	AoraRenderer::~AoraRenderer()
	{
		freeCommandBuffers();
	}

	void AoraRenderer::recreateSwapChain()
	{
		auto extent = aoraWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = aoraWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(aoraDevice.device());

		if (aoraSwapchain == nullptr)
		{
			aoraSwapchain = std::make_unique<AoraSwapchain>(aoraDevice, extent);
		}
		else
		{
			std::shared_ptr<AoraSwapchain> oldSwapchain = std::move(aoraSwapchain);
			aoraSwapchain = std::make_unique<AoraSwapchain>(aoraDevice, extent, oldSwapchain);

			if (!oldSwapchain->compareSwapFormats(*aoraSwapchain.get()))
			{
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
		}

		// TODO: we'll come back to this in just a moment
	}

	void AoraRenderer::createCommandBuffers()
	{ 
		commandBuffers.resize(AoraSwapchain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = aoraDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(aoraDevice.device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void AoraRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			aoraDevice.device(),
			aoraDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data()
		);

		commandBuffers.clear();
	}

	VkCommandBuffer AoraRenderer::beginFrame()
	{
		assert(!bIsFrameStarted && "Can't call beginFrame while already in progress");
		
		auto result = aoraSwapchain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		bIsFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to recording command buffer!");
		}

		return commandBuffer;
	}

	void AoraRenderer::endFrame()
	{
		assert(bIsFrameStarted && "Can't call endFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}

		auto result = aoraSwapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || aoraWindow.wasWindowResized())
		{
			aoraWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		bIsFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % AoraSwapchain::MAX_FRAMES_IN_FLIGHT;
	}

	void AoraRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(bIsFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = aoraSwapchain->getRenderPass();
		renderPassInfo.framebuffer = aoraSwapchain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = aoraSwapchain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(aoraSwapchain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(aoraSwapchain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, aoraSwapchain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void AoraRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(bIsFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

}