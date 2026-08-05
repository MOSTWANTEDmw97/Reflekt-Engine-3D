#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 position {0.0f};
	glm::quat rotation {glm::quat(1.0f, 0.0f, 0.0f, 0.0f)};
	glm::vec3 scale {1.0f};

	glm::vec3 Forward {0.0f, 0.0f, 1.0f};
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 Right{ 1.0f, 0.0f, 0.0f };

	glm::mat4 GetModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model *= glm::mat4_cast(rotation);
		model = glm::scale(model, scale);
		return model;
	}

	void SetPosition(const glm::vec3& pos) { position = pos; }
	void SetRotation(const glm::quat& rot) { rotation = rot; UpdateDirectionVectors(); }
	void SetScale(const glm::vec3& scl) { scale = scl; }

	void SetRotationFromEuler(const glm::vec3& eulerAngles)
	{ 
		rotation = glm::quat(eulerAngles);
		UpdateDirectionVectors();
	}

	void UpdateDirectionVectors()
	{
		Forward = glm::normalize(rotation * glm::vec3(0.0f, 0.0f, 1.0f));
		Up = glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
		Right = glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
	}

};

#endif // !TRANSFORM_H
