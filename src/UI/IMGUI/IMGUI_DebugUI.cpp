#include<imgui.h>
#include<imgui_impl_opengl3.h>
#include<imgui_impl_glfw.h>
#include<stdio.h>
#include<vector>
#include"UI/IMGUI/IMGUI_DebugUI.h"

void IMGUI_DebugUI::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void IMGUI_DebugUI::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void IMGUI_DebugUI::RenderUI()
{// Hierarchy
	ImGui::Begin("Hierarchy");
	for (int i = 0; i < names.size(); i++)
	{
		if (ImGui::Selectable(names[i].c_str(), selectedIndex == i))
		{
			selectedIndex = i;
		}
	}
	ImGui::End();

	// Inspector
	ImGui::Begin("Inspector");
	if (selectedIndex >= 0)
	{
		inspectors[selectedIndex]();
	}
	ImGui::End();

}

void IMGUI_DebugUI::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void IMGUI_DebugUI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

