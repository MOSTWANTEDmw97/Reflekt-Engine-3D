#pragma once
#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H

#include<vector>
#include"Model.h"
#include"Camera.h"

struct RenderEntry
{
	Model* model;
	std::vector<glm::mat4> instanceTransforms;
	bool instanced = false;
};

class RenderPipeline
{
	const Camera* activeCamera = nullptr;
	public:
		std::vector<RenderEntry> opaqueQueue;
		std::vector<RenderEntry> transparentQueue;

		void AddModel(Model& model, const bool isInstanced = false, const std::vector<glm::mat4>& transforms = {});
		void SetCamera(const Camera* camera) { activeCamera = camera; }
		void DrawAll();
		void DrawOpaque();
		void DrawTransparent();


};

#endif // !RENDERPIPELINE_H
