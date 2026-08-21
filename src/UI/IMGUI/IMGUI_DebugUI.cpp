#include<imgui.h>
#include<imgui_impl_opengl3.h>
#include<imgui_impl_glfw.h>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include<iostream>
#include"UI/IMGUI/IMGUI_DebugUI.h"


struct PerfStats
{
	float fps;
	float frameTimeMs;
	float fps1Low;
	float fps01Low;
};

PerfStats CalculatePerfStats(const std::vector<float>& frameTimes)
{
	static float fpsCache = 0.0f;
	static float timer = 0.0f;

	PerfStats stats{};
	if (frameTimes.empty()) return stats;

	float latest = frameTimes.back();
	timer += latest;

	// Update only every 0.1s
	if (timer >= 0.2f)
	{
		fpsCache = 1.0f / latest;
		timer = 0.0f;
	}

	stats.fps = fpsCache;
	stats.frameTimeMs = latest * 1000.0f;

	std::vector<float> sorted = frameTimes;
	std::sort(sorted.begin(), sorted.end());
	int n = (int)sorted.size();

	float p99 = sorted[(int)(n * 0.99f)];
	float p999 = sorted[(int)(n * 0.999f)];

	stats.fps1Low = 1.0f / p99;
	stats.fps01Low = 1.0f / p999;

	return stats;
}

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

void IMGUI_DebugUI::RenderPerfMetrics(const std::vector<float>& frameTime)
{
	PerfStats stats = CalculatePerfStats(frameTime);
	ImGui::SetNextWindowPos(ImVec2(1700, 850));
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("PerfOverlay", nullptr,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	ImGui::Text("FPS: %.1f", stats.fps);
	ImGui::Text("Frame Time: %.2f ms", stats.frameTimeMs);
	ImGui::Text("1%% Low FPS: %.1f", stats.fps1Low);
	ImGui::Text("0.1%% Low FPS: %.1f", stats.fps01Low);

	ImGui::End();
}

void IMGUI_DebugUI::RenderCameraPosition(glm::vec3 pos)
{
	ImGui::SetNextWindowPos(ImVec2(1700, 950));
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Pos:", nullptr,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", pos.x, pos.y, pos.z);

	//std::cout<< "x: "<< pos.x << "y: " << pos.y << "z: " << pos.z << std::endl;
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

