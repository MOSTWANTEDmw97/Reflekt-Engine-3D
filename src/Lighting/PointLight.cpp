#include"Lighting/PointLight.h"

PointLight::PointLight(PointLightManager& mgr,
	Transform& transform,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const float constant, const float linear, const float quadratic)
	:manager(mgr), transform(transform)
{
	PointLightData light
	{
		glm::vec4(transform.position, 1.0f),
		glm::vec4(ambient, constant),
		glm::vec4(diffuse, linear),
		glm::vec4(specular, quadratic)
	};
	manager.Add(light);
	index = (int)manager.lights.size() - 1;
	manager.Upload();
	manager.Bind();
}

void PointLight::SetPosition(const glm::vec3& position)
{
	transform.position = position;
	manager.lights[index].position = glm::vec4(position, 1.0f);
	manager.Upload();
}

void PointLight::SetAmbient(const glm::vec3& ambient)
{
	manager.lights[index].ambient_And_Constant = glm::vec4(ambient, manager.lights[index].ambient_And_Constant.w);
	manager.Upload();
}

void PointLight::SetDiffuse(const glm::vec3& diffuse)
{
	manager.lights[index].diffuse_And_Linear = glm::vec4(diffuse, manager.lights[index].diffuse_And_Linear.w);
	manager.Upload();
}

void PointLight::SetSpecular(const glm::vec3& specular)
{
	manager.lights[index].specular_And_Quadratic = glm::vec4(specular, manager.lights[index].specular_And_Quadratic.w);
	manager.Upload();
}

void PointLight::SetAttenuation(const float constant, const float linear, const float quadratic)
{
	manager.lights[index].ambient_And_Constant.w = constant;
	manager.lights[index].diffuse_And_Linear.w = linear;
	manager.lights[index].specular_And_Quadratic.w = quadratic;
	manager.Upload();
}

void PointLight::SetAll(const glm::vec3& position,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const float constant, const float linear, const float quadratic)
{
	transform.position = position;
	manager.lights[index].position = glm::vec4(position, 1.0f);

	manager.lights[index].ambient_And_Constant = glm::vec4(ambient, constant);
	manager.lights[index].diffuse_And_Linear = glm::vec4(diffuse, linear);
	manager.lights[index].specular_And_Quadratic = glm::vec4(specular, quadratic);

	manager.Upload();
}
