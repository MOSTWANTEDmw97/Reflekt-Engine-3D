#include<gtc/type_ptr.hpp>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<cstring>

#include"Camera.h"

Camera::Camera(glm::vec3 position, float fov, float clip_Near, float clip_Far)
	:Forward(glm::vec3(0.0f, 0.0f, -1.0f)),
	WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	Yaw(-90.0f), Pitch(0.0f),
	MovementSpeed(2.5f),
	MouseSensitivity(0.1f),
	FOV(fov)
{
	Position = position;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Forward, Up);
}
glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
}

void Camera::BindToShader(Shader& shader, const glm::mat4& model, float aspectRatio) const
{
	glm::mat4 view = GetViewMatrix();
	glm::mat4 proj = GetProjectionMatrix(aspectRatio);

	shader.Use();
	shader.SetMat4("model", model);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", proj);

}

void Camera::SetUpDir(glm::vec3 up)
{
	WorldUp = up;
	UpdateCameraVectors();
}
void Camera::SetYawPitch(float yaw, float pitch)
{
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}
void Camera::SetFOV(float fov)
{
	FOV = fov;
}
void Camera::SetClipPlanes(float near, float far)
{
	Clip_Near = near;
	Clip_Far = far;
}

void Camera::SetSpeed(float speed)
{
	MovementSpeed = speed;
}
void Camera::SetSensitivity(float sensitivity)
{
	MouseSensitivity = sensitivity;
}


void Camera::ProcessKeyboardInputs(const char* direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (strcmp(direction, "FORWARD") == 0) Position += Forward * velocity;
	if (strcmp(direction, "BACKWARD") == 0) Position -= Forward * velocity;
	if (strcmp(direction, "LEFT") == 0) Position -= Right * velocity;
	if (strcmp(direction, "RIGHT") == 0) Position += Right * velocity;
}

void Camera::ProcessMouseInputs(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

// Private
void Camera::UpdateCameraVectors()
{
	glm::vec3 forward;
	forward.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	forward.y = sin(glm::radians(Pitch));
	forward.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Forward = glm::normalize(forward);

	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}
