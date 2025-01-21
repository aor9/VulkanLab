#pragma once

#include "AoraCamera.h"
#include "AoraDevice.h"
#include "AoraGameObject.h"
#include "AoraRenderer.h"

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

		SimpleRenderSystem(AoraDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator = (const SimpleRenderSystem&) = delete;

		void renderGameObjects(
			VkCommandBuffer commandBuffer, 
			std::vector<AoraGameObject>& gameObjects, 
			const AoraCamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		AoraDevice& aoraDevice;

		std::unique_ptr<AoraPipeline> aoraPipeline;
		VkPipelineLayout pipelineLayout;
	};
}