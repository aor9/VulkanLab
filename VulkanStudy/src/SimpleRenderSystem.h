#pragma once

#include "AoraCamera.h"
#include "AoraDevice.h"
#include "AoraGameObject.h"
#include "AoraRenderer.h"
#include "AoraFrameInfo.h"

// std
#include <memory>
#include <vector>

namespace aor
{
	class SimpleRenderSystem
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		SimpleRenderSystem(AoraDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator = (const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo, std::vector<AoraGameObject>& gameObjects);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		AoraDevice& aoraDevice;

		std::unique_ptr<AoraPipeline> aoraPipeline;
		VkPipelineLayout pipelineLayout;
	};
}