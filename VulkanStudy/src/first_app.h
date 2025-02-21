#pragma once

#include "AoraWindow.h"
#include "AoraDevice.h"
#include "AoraGameObject.h"
#include "AoraRenderer.h"
#include "AoraDescriptors.h"

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

		AoraWindow aoraWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		AoraDevice aoraDevice{ aoraWindow };
		AoraRenderer aoraRenderer{ aoraWindow, aoraDevice };

		// note: order of declarations matters
		std::unique_ptr<AoraDescriptorPool> globalPool{};
		std::vector<AoraGameObject> gameObjects;

		const float MAX_FRAME_TIME = 240.f;
	};
}