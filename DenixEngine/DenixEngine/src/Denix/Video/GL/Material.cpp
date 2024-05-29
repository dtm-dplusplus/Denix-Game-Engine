#include "Material.h"
#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{
	Material::Material(const ObjectInitializer& _objInit) : Object({ _objInit })
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
		m_BaseParam = BaseMatParam();
		m_NormalParam = NormalMatParam();
	}
	Material::Material(Ref<Material> _other)
	{
		m_Shader = _other->m_Shader;
		m_BaseParam = _other->m_BaseParam;
		m_NormalParam = _other->m_NormalParam;
		m_SpecularIntensity = _other->m_SpecularIntensity;
		m_SpecularPower = _other->m_SpecularPower;
	}

	void DefaultMaterial::UploadUniforms()
	{
		Material::UploadUniforms();

		const BaseMatParam& base = GetBaseParam();
		glUniform1i(m_Shader->GetUniform("u_Material.Base.IsTexture"), base.IsTexture);

		if (base.IsTexture && base.Texture)
		{
			base.Texture->Bind();

			// Texture Settings need to move to the material/texture
			//glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_WRAP_S, renderComp->m_TextureSettings.WrapMode);
			//glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_WRAP_T, renderComp->m_TextureSettings.WrapMode);
			//glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, renderComp->m_TextureSettings.FilterMode);
			//glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, renderComp->m_TextureSettings.FilterMode);
		}
		else
		{
			glUniform3f(m_Shader->GetUniform("u_Material.Base.Color"),
				base.Color.r, base.Color.g, base.Color.b);


			glUniform1f(m_Shader->GetUniform("u_Material.SpecularIntensity"), GetSpecularIntensity());
			glUniform1f(m_Shader->GetUniform("u_Material.SpecularPower"), GetSpecularPower());
		}
	}
}