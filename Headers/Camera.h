#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

#include"Shader.h"

class Camera
{
	public:
		glm::vec3 Position;
		glm::vec3 Forward;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//Euler angles
		float Yaw;
		float Pitch;

		//Settings
		float MovementSpeed;
		float MouseSensitivity;

		float FOV;
		float Clip_Near;
		float Clip_Far;

		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
			float fov = 60.0f,
			float clip_Near = 0.1f,
			float clip_Far = 100.0f);

		//Matrices
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(float aspectRatio) const;
		//glm::mat4 GetMVP(glm::mat4 model, float aspectRatio) const; //Model View Proj

		void BindToShader(Shader& shader, const glm::mat4&	 model, float aspectRatio) const;

		//Helpers and setters
		void SetUpDir(glm::vec3 up);
		void SetYawPitch(float yaw, float pitch);
		void SetFOV(float fov);
		void SetClipPlanes(float near, float far);
		void SetSpeed(float speed);
		void SetSensitivity(float sensitivity);

		//Inputs
		void ProcessKeyboardInputs(const char* dir, float deltaTime);
		void ProcessMouseInputs(float xOffset, float yOffset, bool constrainPitch = true);

	private:
		void UpdateCameraVectors();
};

#endif // !CAMERA_H
