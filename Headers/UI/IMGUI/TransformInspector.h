#pragma once
#ifndef TRANSFORMINSPECTOR_H
#define TRANSFORMINSPECTOR_H

#include<gtc/type_ptr.hpp>
#include<glm.hpp>
#include<imgui.h>

#include"Math/Transform.h"

inline void DrawTransformInspector(Transform& t)
{
    ImGui::Text("Transform");
    ImGui::SliderFloat3("Position", glm::value_ptr(t.position), -100.0f, 100.0f);

    glm::vec3 euler = glm::degrees(glm::eulerAngles(t.rotation));
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(euler), -180.0f, 180.0f))
    {
        t.SetRotationFromEuler(glm::radians(euler));
    }

    ImGui::SliderFloat3("Scale", glm::value_ptr(t.scale), 0.001f, 10.0f);
}

#endif // !TRANSFORMINSPECTOR_H
