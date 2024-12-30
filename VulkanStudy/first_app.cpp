#include "first_app.h"

//std
#include <stdexcept>

namespace aor
{
	FirstApp::FirstApp()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(aoraDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run()
	{
		while (!aoraWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}

	void FirstApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(aoraDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!!");
		}
	}

	void FirstApp::createPipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		AoraPipeline::defaultPipelineConfigInfo(
			pipelineConfig,
			aoraSwapchain.width(),
			aoraSwapchain.height());
		
		pipelineConfig.renderPass = aoraSwapchain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		aoraPipeline = std::make_unique<AoraPipeline>(
			aoraDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig
		);
	}

	void FirstApp::createCommandBuffers()
	{
		
	}

	void FirstApp::drawFrame()
	{

	}
}