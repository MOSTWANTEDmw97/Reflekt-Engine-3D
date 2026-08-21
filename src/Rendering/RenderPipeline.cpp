#include"Rendering/RenderPipeline.h"
#include<algorithm>
#include<glad/glad.h>

void RenderPipeline::AddModel(Model& model)
{
    for (const Mesh& mesh : model.GetMeshes())
    {
        if (mesh.GetMaterial().surfaceType == SurfaceType::Transparent)
        {
            transparentQueue.push_back(&model);
        }
        else
        {
            opaqueQueue.push_back(&model);
        }
    }
}

void RenderPipeline::DrawOpaque()
{
    //glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);

    for (Model* model : opaqueQueue)
    {
        model->Draw();
    }

}

void RenderPipeline::DrawTransparent()
{
    std::sort(transparentQueue.begin(), transparentQueue.end(),
        [&](Model* a, Model* b)
        {
            float distA = glm::length(activeCamera->transform.position - a->transform.position);
            float distB = glm::length(activeCamera->transform.position - b->transform.position);
            return distA > distB; // farthest first
        });

    //glEnable(GL_BLEND);
    //glDepthMask(GL_FALSE);

    for (Model* model : transparentQueue)
    {
        model->Draw();
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