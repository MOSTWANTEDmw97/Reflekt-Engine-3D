#include"Rendering/RenderPipeline.h"
#include<algorithm>
#include<glad/glad.h>
#include<iostream>

//Both instanced and non instanced
void RenderPipeline::AddMeshRenderer(MeshRenderer& meshRenderer, const bool isInstanced, const std::vector<glm::mat4>& transforms)
{

    for (Mesh& mesh : meshRenderer.model->GetMeshes())
    {
        if (!isInstanced)
        {
            RenderEntry entry(meshRenderer, {}, false);

            if (meshRenderer.GetMaterial().surfaceType == SurfaceType::Transparent)
            {
                transparentQueue.push_back(entry);
            }
            else
            {
                opaqueQueue.push_back(entry);
            }
        }

        else if (isInstanced)
        {
            RenderEntry entry(meshRenderer, transforms, true);
            for (Mesh& mesh : meshRenderer.model->GetMeshes())
            {
                mesh.SetupInstanceBuffer(transforms);
            }
            if (meshRenderer.GetMaterial().surfaceType == SurfaceType::Transparent)
            {
                transparentQueue.push_back(entry);
            }
            else
            {
                opaqueQueue.push_back(entry);
            }
        }
       // std::cout << "Entry instanced: " << opaqueQueue.size() << std::endl;
    }
}

void RenderPipeline::DrawOpaque(Shader* overrideShader)
{
    for (auto& entry : opaqueQueue)
    {
        entry.meshRenderer.Draw(overrideShader); // Unbind texture after drawing
    }

}

void RenderPipeline::DrawTransparent(Shader* overrideShader)
{
    std::sort(transparentQueue.begin(), transparentQueue.end(),
        [&](RenderEntry a, RenderEntry b)
        {
            float distA = glm::length(activeCamera->transform.position - a.meshRenderer.gameObject->transform.position);
            float distB = glm::length(activeCamera->transform.position - b.meshRenderer.gameObject->transform.position);
            return distA > distB; // farthest first
        });

    for (auto entry : transparentQueue)
    {
        entry.meshRenderer.Draw(overrideShader);
    }
}

void RenderPipeline::DrawAll(Shader* overrideShader)
{
    DrawOpaque(overrideShader);
    DrawTransparent(overrideShader);
}
