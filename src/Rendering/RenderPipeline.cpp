#include"Rendering/RenderPipeline.h"
#include<algorithm>
#include<glad/glad.h>
#include<iostream>

//Both instanced and non instanced
void RenderPipeline::AddModel(Model& model, const bool isInstanced, const std::vector<glm::mat4>& transforms)
{
    for (Mesh& mesh : model.GetMeshes())
    {
        if (!isInstanced)
        {
            RenderEntry entry(&model, {}, false);

            if (mesh.GetMaterial().surfaceType == SurfaceType::Transparent)
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
            RenderEntry entry(&model, transforms, true);
            for (Mesh& mesh : model.GetMeshes())
            {
                mesh.SetupInstanceBuffer(transforms);
            }
            if (mesh.GetMaterial().surfaceType == SurfaceType::Transparent)
            {
                transparentQueue.push_back(entry);
            }
            else
            {
                opaqueQueue.push_back(entry);
            }
        }
        //std::cout << "Entry instanced: " << isInstanced << std::endl;
    }
}

void RenderPipeline::DrawOpaque()
{
    //glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);

    for (auto& entry : opaqueQueue)
    {
        if (entry.instanced)
        {
            entry.model->DrawInstanced(entry.instanceTransforms);
        }
        else
        {
            entry.model->Draw();
        }
        //std::cout << "Draw instance: " << entry.instanced << std::endl;
    }

}

void RenderPipeline::DrawTransparent()
{
    std::sort(transparentQueue.begin(), transparentQueue.end(),
        [&](RenderEntry a, RenderEntry b)
        {
            float distA = glm::length(activeCamera->transform.position - a.model->transform.position);
            float distB = glm::length(activeCamera->transform.position - b.model->transform.position);
            return distA > distB; // farthest first
        });

    //glEnable(GL_BLEND);
    //glDepthMask(GL_FALSE);

    for (auto entry : transparentQueue)
    {
        if (entry.instanced)
        {
            entry.model->DrawInstanced(entry.instanceTransforms);
        }
        else
        {
            entry.model->Draw();
        }
    }

    // Restore state
    //glDepthMask(GL_TRUE);
    //glDisable(GL_BLEND);
}

void RenderPipeline::DrawAll()
{
    DrawOpaque();
    DrawTransparent();
}