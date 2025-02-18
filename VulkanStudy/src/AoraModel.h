#pragma once

#include "AoraDevice.h"
#include "AoraBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace aor
{
	class AoraModel
	{
	public:

		struct Vertex
		{
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Data
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		AoraModel(AoraDevice &device, const AoraModel::Data &data);
		~AoraModel();

		AoraModel(const AoraModel&) = delete;
		AoraModel& operator = (const AoraModel&) = delete;

		static std::unique_ptr<AoraModel> createModelFromFile(AoraDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		AoraDevice& aoraDevice;

		std::unique_ptr<AoraBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool bHasIndexBuffer = false;
		std::unique_ptr<AoraBuffer> indexBuffer;
		uint32_t indexCount;
	};
}