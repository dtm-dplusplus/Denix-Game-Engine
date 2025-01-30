/**
 * @file RenderComponent.h
 * @brief Definition of the RenderComponent class in the Denix engine.
 */
#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Texture.h"

namespace Denix
{
    /**
     * @class RenderComponent
     * @brief Handles rendering-related functionality for an actor.
     */
    class RenderComponent : public Component
    {
    public:
        /**
         * @brief Default constructor for RenderComponent.
         */
        RenderComponent();

        /**
         * @brief Default destructor for RenderComponent.
         */
        ~RenderComponent() override = default;

        /**
         * @brief Gets the material associated with this component.
         * @return A reference to the material.
         */
        Ref<Material> GetMaterial() const { return m_Material; }

        /**
         * @brief Sets the material for this component.
         * @param _material A reference to the new material.
         */
        void SetMaterial(const Ref<Material>& _material);

        bool m_IsVisible; ///< Indicates whether the component is visible.

    private:
        /**
         * @brief Serializes the component.
         * @param _out YAML emitter for serialization.
         */
        void Serialize(YAML::Emitter& _out) override;

        /**
         * @brief Deserializes the component.
         * @param _in YAML node for deserialization.
         */
        void Deserialize(const YAML::Node& _in) override;

        Ref<Material> m_Material; ///< Reference to the material used for rendering.

        friend class RendererSubsystem; ///< Grants RendererSubsystem access.
        friend class UISubsystem; ///< Grants UISubsystem access.
    };
}
