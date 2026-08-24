#pragma once
#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H

#include<vector>
#include"MeshRenderer.h"
#include"Camera.h"


struct RenderEntry
{
	MeshRenderer* meshRenderer;
	std::vector<glm::mat4> instanceTransforms;
	bool instanced = false;
};

class RenderPipeline
{
	const Camera* activeCamera = nullptr;
	public:
		std::vector<RenderEntry> opaqueQueue;
		std::vector<RenderEntry> transparentQueue;

		void AddMeshRenderer(MeshRenderer& meshRenderer, const bool isInstanced = false, const std::vector<glm::mat4>& transforms = {});

		void SetCamera(const Camera* camera) { activeCamera = camera; }

		void DrawAll(Shader* overrideShader = nullptr);
		void DrawOpaque(Shader* overrideShader = nullptr);
		void DrawTransparent(Shader* overrideShader = nullptr);

		int ReturnMeshRendererCount() { return static_cast<int>(opaqueQueue.size() + transparentQueue.size());};
		int ReturnOpaqueQueueCount() { return static_cast<int>(opaqueQueue.size()); };
		int ReturnTransparentQueueCount() { return static_cast<int>(transparentQueue.size()); };

};

#endif // !RENDERPIPELINE_H
