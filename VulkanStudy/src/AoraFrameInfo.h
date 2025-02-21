#pragma once

#include "AoraCamera.h"

//lib
#include <vulkan/vulkan.h>

namespace aor
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		AoraCamera& camera;
		VkDescriptorSet globlaDescriptorSet;
	};
}