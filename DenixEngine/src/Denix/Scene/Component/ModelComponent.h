/**
 * @file ModelComponent.h
 * @brief Defines the ModelComponent class for handling 3D models in the scene.
 */

#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
    /**
     * @class ModelComponent
     * @brief A component that stores a reference to a 3D model.
     *
     * This component allows an actor to have an associated 3D model, which can be used for rendering.
     */
    class ModelComponent final : public Component
    {
    public:
        /**
         * @brief Default constructor for ModelComponent.
         */
        ModelComponent();

        /**
         * @brief Gets the model associated with this component.
         * @return A reference to the model.
         */
        Ref<Model> GetModel() const { return m_Model; }

        /**
         * @brief Sets the model for this component.
         * @param _model A reference to the model to be set.
         */
        void SetModel(const Ref<Model>& _model) { m_Model = _model; }

    private:
        /**
         * @brief Serializes the component data to YAML format.
         * @param _out The YAML emitter used for serialization.
         */
        void Serialize(YAML::Emitter& _out) override;

        /**
         * @brief Deserializes the component data from YAML format.
         * @param _in The YAML node containing serialized data.
         */
        void Deserialize(const YAML::Node& _in) override;

        Ref<Model> m_Model; ///< The 3D model associated with this component.

        friend class Actor; ///< Allows Actor to access private members.
        friend class RendererSubsystem; ///< Allows RendererSubsystem to access private members.
        friend class UISubsystem; ///< Allows UISubsystem to access private members.
    };
}