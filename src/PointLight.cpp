#include"PointLight.h"

PointLight::PointLight(PointLightManager& mgr,
	const glm::vec3& position,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const float constant, const float linear, const float quadratic)
	:manager(mgr)
{
	PointLightData light
	{
		position, constant,
		ambient, linear,
		diffuse, quadratic,
		specular, 1.0f
	};
	manager.Add(light);
	index = (int)manager.lights.size() - 1;
	manager.Upload();
	manager.Bind();
}

void PointLight::SetPosition(const glm::vec3& position)
{
	manager.lights[index].position = position;
	manager.Upload();
}

void PointLight::SetAmbient(const glm::vec3& ambient)
{
	manager.lights[index].ambient = ambient;
	manager.Upload();
}

void PointLight::SetDiffuse(const glm::vec3& diffuse)
{
	manager.lights[index].diffuse = diffuse;
	manager.Upload();
}

void PointLight::SetSpecular(const glm::vec3& specular)
{
	manager.lights[index].specular = specular;
	manager.Upload();
}

void PointLight::SetAttenuation(const float constant, const float linear, const float quadratic)
{
	manager.lights[index].constant = constant;
	manager.lights[index].linear = linear;
	manager.lights[index].quadratic = quadratic;
	manager.Upload();
}

void PointLight::SetAll(const glm::vec3& position,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const float constant, const float linear, const float quadratic)
{
	manager.lights[index].position = position;

	manager.lights[index].ambient = ambient;
	manager.lights[index].diffuse = diffuse;
	manager.lights[index].specular = specular;

	manager.lights[index].constant = constant;
	manager.lights[index].linear = linear;
	manager.lights[index].quadratic = quadratic;

	manager.Upload();
}
