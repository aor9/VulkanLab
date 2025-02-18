#pragma once

#include "AoraModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include<memory>

namespace aor
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();

		glm::mat3 normalMatrix();
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
		TransformComponent transform{};

	private:
		AoraGameObject(id_t objId) : id{ objId } {}

		id_t id;
	};
}