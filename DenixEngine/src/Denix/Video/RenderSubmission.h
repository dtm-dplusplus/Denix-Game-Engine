#pragma once
#include "Denix/Core.h"
#include "GL/Material.h"
#include "GL/Model.h"

namespace Denix
{
    struct RenderSubmission
    {
        RenderSubmission() = default;

        RenderSubmission(const Ref<Material>& _mat, const Ref<Model>& _model, const glm::mat4& _transformModel)
            : Mat(_mat), Model(_model), TransformModel(_transformModel)
        {
        }

        Ref<Material> Mat;
        Ref<Model> Model;
        glm::mat4 TransformModel;
    };

    struct CameraSubmission
    {
        CameraSubmission() = default;

        CameraSubmission(const glm::mat4& _projection, const glm::mat4& _view)
            : Projection(_projection), View(_view)
        {
        }

        glm::mat4 Projection;
        glm::mat4 View;
    };
}
