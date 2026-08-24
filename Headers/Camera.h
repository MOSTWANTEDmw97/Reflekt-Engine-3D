#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/quaternion.hpp>

#include"Graphics/Shader.h"
#include"Math/Transform.h"
#include"Rendering/Skybox.h"

class Camera
{
	public:
		Transform transform;
		Skybox* skybox = nullptr;
		//Euler angles

		float FOV;
		float Clip_Near;
		float Clip_Far;

		Camera(const Transform& t,
			float fov = 60.0f,
			float clip_Near = 0.1f,
			float clip_Far = 100.0f,
			Skybox* skyboxInput = nullptr);

		//Matrices
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(float aspectRatio) const;
		//glm::mat4 GetMVP(glm::mat4 model, float aspectRatio) const; //Model View Proj

		void BindToShader(Shader& shader, const glm::mat4&	 model, float aspectRatio) const;

		//Helpers and setters
		void SetPosition(const glm::vec3& pos);
		void SetRotation(const glm::quat& rot);
		void SetFOV(float fov);
		void SetClipPlanes(float near, float far);
		void SetSkybox(Skybox* skyboxInput) { skybox = skyboxInput; };
		Skybox ReturnSkybox() { return *skybox; };
		
};

#endif // !CAMERA_H
