#include<gtc/type_ptr.hpp>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<cstring>

#include"Camera.h"

Camera::Camera(const Transform& t, float fov, float clip_Near, float clip_Far)
	:transform(t), FOV(fov), Clip_Near(clip_Near), Clip_Far(clip_Far) {}

glm::mat4 Camera::GetViewMatrix() const
{
	//+Z forward
	return glm::lookAt(
		transform.position,
		transform.position + transform.Forward,
		transform.Up
	);
}
glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(glm::radians(FOV), aspectRatio, Clip_Near, Clip_Far);
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

void Camera::SetPosition(const glm::vec3& pos)
{
	transform.SetPosition(pos);
}
void Camera::SetRotation(const glm::quat& rot)
{
	transform.SetRotation(rot);
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

// Private

