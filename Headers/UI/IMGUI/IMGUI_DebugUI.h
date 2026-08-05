#pragma once
#ifndef IMGUI_DEBUGUI_H
#define IMGUI_DEBUGUI_H

#include<glfw/glfw3.h>
#include<glm.hpp>
#include<vector>
#include<functional>
#include<string>

class IMGUI_DebugUI
{
	public:
		void Init(GLFWwindow* window);
		void BeginFrame();
		void RenderUI();
		void EndFrame();
		void Shutdown();


		template<typename T>
		void RegisterObject(T* obj, const std::string& displayName)
		{
			names.push_back(displayName);
			inspectors.push_back([obj]() { obj->DrawInspector(); });
		}

	private:
		std::vector<std::string> names;
		std::vector<std::function<void()>> inspectors;
		int selectedIndex = -1;
};

#endif // !IMGUI_DEBUGUI_H
