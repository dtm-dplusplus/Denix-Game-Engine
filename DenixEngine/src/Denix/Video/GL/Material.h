#pragma once

#include "Denix/Core/BaseObject.h"
#include "Denix/Core/Object.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Shader.h"


namespace Denix
{
    class Material : public BaseObject
    {
    public:
        Material(const ObjectInit& _objInit = {"Material"});
        Material(const Ref<Material>& _other);

        // Serialization
        void Serialize(YAML::Emitter& _out) override;
        void Deserialize(const YAML::Node& _in) override;

        Ref<Shader> GetShader() const { return m_Shader; }
        void SetShader(const Ref<Shader>& _shader) { m_Shader = _shader; }

        // Albedo color or texture
        void SetBaseColor(const glm::vec3& _color) { m_BaseColor = _color; }
        void SetBaseTexture(const Ref<Texture>& _texture);
        Ref<Texture> GetBaseTexture() const { return m_BaseTexture; }
        Ref<Texture>& GetBaseTexture() { return m_BaseTexture; }

        bool CheckBaseType()
        {
            m_IsBaseTexture = IsValid(m_BaseTexture);
            return m_IsBaseTexture;
        }

        /**
         *  @brief Get the base type of the material
         * @return true if the base is a texture, false if it is a color
         */
        bool IsBaseATexture() const { return m_IsBaseTexture; }

        void ClearBaseTexture()
        {
            m_BaseTexture = nullptr;
            m_IsBaseTexture = false;
        }

        glm::vec3 m_BaseColor = glm::vec3(0.8f);
        
        
        bool m_MultiplyBase = false;

        float m_TextureTiling = 1.0f;
        TextureSettings m_TextureSettings;

    private:
        // Base color or texture
        bool m_IsBaseTexture = false;
        Ref<Texture> m_BaseTexture;

        Ref<Shader> m_Shader;

        friend class RenderComponent;
        friend class RendererSubsystem;
        friend class UISubsystem;
    };
}
