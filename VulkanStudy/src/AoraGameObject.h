#pragma once

#include "AoraModel.h"

#include<memory>

namespace aor
{
	struct Transform2dComponent
	{
		glm::vec2 translation{};	 // position offset
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;
		
		glm::mat2 mat2() 
		{ 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c, s}, {-s, c} };

			glm::mat2 scaleMatrix{ {scale.x, 0.f}, {.0f, scale.y} };
			return rotMatrix * scaleMatrix; 
		}
	};

	class AoraGameObject
	{
	public:
		using id_t = unsigned int;

		static AoraGameObject createGameObject()
		{
			static id_t currentId = 0;
			return AoraGameObject{ currentId++ };
		}

		AoraGameObject(const AoraGameObject&) = delete;
		AoraGameObject& operator=(const AoraGameObject&) = delete;
		AoraGameObject(AoraGameObject&&) = default;
		AoraGameObject& operator=(AoraGameObject&&) = default;

		id_t getId() const { return id; };
		
		std::shared_ptr<AoraModel> model{};
		glm::vec3 color{};
		Transform2dComponent transform2d{};

	private:
		AoraGameObject(id_t objId) : id{ objId } {}

		id_t id;
	};
}