#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include<gtc/type_ptr.hpp>
#include<glm.hpp>
#include<imgui.h>

#include"Math/Transform.h"
#include"Material.h"

inline void DrawTransformInspector(Transform& t)
{
    ImGui::Text("Transform");
    ImGui::DragFloat3("Position", glm::value_ptr(t.position), 0.1f);
    glm::vec3 euler = glm::degrees(glm::eulerAngles(t.rotation));
    if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler), 1.0f))
    {
        t.SetRotationFromEuler(glm::radians(euler));
    }
    ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), 0.01f, 0.001f, 10.0f);
}

inline void DrawMaterialInspector(Material& material)
{
    ImGui::Separator();
    ImGui::Text("Material");

}



#endif // !TRANSFORMINSPECTOR_H
