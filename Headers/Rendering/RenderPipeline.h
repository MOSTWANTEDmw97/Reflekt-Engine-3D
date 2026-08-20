#pragma once
#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H

#include<vector>
#include"Model.h"
#include"Camera.h"

class RenderPipeline
{
	const Camera* activeCamera = nullptr;
	public:
		std::vector<Model*> opaqueQueue;
		std::vector<Model*> transparentQueue;

		void AddModel(Model& model);
		void SetCamera(const Camera* camera) { activeCamera = camera; }
		void DrawAll();
		void DrawOpaque();
		void DrawTransparent();


};

#endif // !RENDERPIPELINE_H
