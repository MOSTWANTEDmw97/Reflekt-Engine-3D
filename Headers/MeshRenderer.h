#pragma once
#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include"Component.h"
#include"Model.h"
#include"Graphics/Material.h"
#include"Graphics/Shader.h"
#include"GameObject.h"

class MeshRenderer : public Component
{
	public:
		Model* model;
		Material* material;
		bool isInstanced = false;
		std::vector<glm::mat4> transforms;

		MeshRenderer(Model& m, bool isInstanced = false, const std::vector<glm::mat4>& transforms = {});
		MeshRenderer(Model& m, Material& mat, bool isInstanced = false, const std::vector<glm::mat4>& transforms = {});
		void SetMaterial(Material& mat) { material = &mat; };
		Material GetMaterial() { return *material; };
		void Draw(Shader* shader = nullptr);
};

#endif // !MESHRENDERER_H
