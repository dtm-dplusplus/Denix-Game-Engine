#include "AssetSubsystem.h"

#include "Denix/Engine.h"
#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Scene/Component/RenderComponent.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Material.h"
#include "Denix/Video/GL/Model.h"
#include "Denix/Asset/Asset.h"
#include "Denix/Audio/AudioClip.h"
#include "yaml-cpp/yaml.h"

namespace Denix
{
	
	void AssetSubsystem::Initialize()
	{
		DE_LOG(LogAsset, Warn, "Asset Subsystem Initializing")
		Subsystem::Initialize();

		// Iniatlize Default Assets
		std::vector<ShaderSource> defaultShaders;
		defaultShaders.emplace_back(R"(Content/Engine/shaders/Default/Vertex.glsl)");
		defaultShaders.emplace_back(R"(Content/Engine/shaders/Default/Fragment.glsl)");
		
		if (Ref<Shader> defShader = LoadShader(defaultShaders, "DefaultShader")) m_DefaultShader = defShader;
		DE_ASSERT(m_DefaultShader, "Default Shader not loaded")
		
		std::vector<ShaderSource> fbShaders;
		fbShaders.emplace_back(R"(Content/Engine/shaders/FB/FBVertex.glsl)");
		fbShaders.emplace_back(R"(Content/Engine/shaders/FB/FBFragment.glsl)");
		if (Ref<Shader> fbShader = LoadShader(fbShaders, "FBShader")) m_FramebufferShader = fbShader;
		DE_ASSERT(m_FramebufferShader, "Framebuffer Shader not loaded")

		if (Ref<Material> defMat = LoadMaterial(MakeRef<Asset>(FileSubsystem::FormatPath(R"(Content/Engine/materials/MAT_Default.asset)")))) m_DefaultMaterial = defMat;
		DE_ASSERT(m_DefaultMaterial, "Default Material not loaded")

		// Search Project directory for assets
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSubsystem::GetContentRoot()))
		{
			const std::string& path = entry.path().string();
			if (entry.is_regular_file())
			{
				Ref<Asset> asset = MakeRef<Asset>(path);

				// DE_ASSET Validation
				if (asset->m_AssetExtension == ".asset")
				{
					const std::string assetData = FileSubsystem::ReadFile(path);

					// Scene Asset
					if (assetData.find("DE_ASSET_SCENE") != std::string::npos)
					{
						// We don't need to load the scene asset here
						m_SceneStore.push_back(asset);
					}

					// Material Asset
					else if (assetData.find("DE_ASSET_MATERIAL") != std::string::npos)
					{
						m_MaterialStore[asset->GetAssetName()] = MakeRef<Material>(asset);
					}
				}

				// NON DE_ASSET Validation
				else
				{
					// Texture Asset
					if (asset->m_AssetExtension == ".png" || asset->m_AssetExtension == ".jpg")
					{
						// Probably not a good idea to load all textures at startup
						LoadTexture(path);
					}
				
					// Shader Check
					
					// Model Check
					else if (asset->m_AssetExtension == ".obj")
					{
						LoadModel(path);
					}
					// Audio Asset
					else if (asset->m_AssetExtension == ".wav")
					{
						LoadAudioClip(asset);
					}
				}
				
				m_AssetStore.push_back(asset);
			}
		}

		if (Ref<Asset> startSceneAsset = GetSceneAsset(FileSubsystem::FormatPath(Engine::GetInstance()->m_Config.StartupScenePath)))
		{
			m_StartupScene = startSceneAsset;
			DE_LOG(LogEngine, Info, "Loaded Engine Config: Startup Scene: {0}", startSceneAsset->GetAssetName())
		}
		else
		{
			DE_LOG(LogEngine, Warn, "Load Engine Config: Startup Scene Not Found")
		}
	    DE_LOG(LogAsset, Info, "Asset Subsystem Initialized")
	}

	void AssetSubsystem::Deinitialize()
	{
		Subsystem::Deinitialize();
		DE_LOG(LogAsset, Trace, "AssetSubsystem Deinitializing")
		
		// Free all Asset
		m_ModelStore.clear();
		
		m_SceneStore.clear();
		m_TextureStore.clear();
		
		m_ShaderStore.clear();

		m_DefaultShader.reset();
		m_FramebufferShader.reset();

		m_MaterialStore.clear();
		m_DefaultMaterial.reset();

		m_AudioClipStore.clear();
		
		m_AssetStore.clear();

		DE_LOG(LogAsset, Trace, "AssetSubsystem Deinitialized")
	}
	
	Ref<Asset> AssetSubsystem::GetAsset(const std::string& _path)
	{
		for (const auto& asset : s_Instance->m_AssetStore)
		{
			if (asset->GetAssetPath() == _path)
			{
				return asset;
			}
		}

		return nullptr;
	}

	Ref<AudioClip> AssetSubsystem::LoadAudioClip(const Ref<Asset>& _audioClipAsset)
	{
		if (s_Instance->m_AudioClipStore.contains(_audioClipAsset->GetAssetPath()))
		{
			DE_LOG(LogAsset, Error, "Load AudioClip: An audio clip name: {} is already loaded", _audioClipAsset->m_AssetName)
			return s_Instance->m_AudioClipStore[_audioClipAsset->GetAssetPath()];
		}

		if (Ref<AudioClip> audioClip = MakeRef<AudioClip>(AssetInit(_audioClipAsset->GetAssetPath())))
		{
			if (!audioClip->Load()) return nullptr;
			
			s_Instance->m_AudioClipStore[_audioClipAsset->GetAssetPath()] = audioClip;
			DE_LOG(LogAsset, Trace, "Audio Clip Loaded: {}", _audioClipAsset->m_AssetName)
			return audioClip;
		}

		return nullptr;
	}

	Ref<AudioClip> AssetSubsystem::GetAudioClip(const std::string& _path)
	{
		std::string path = FileSubsystem::FormatPath(_path);
		DE_LOG(LogAsset, Trace, "Pathhh Audio Clip: {}", path)
		if (!s_Instance->m_AudioClipStore.contains(path))
		{
			DE_LOG(LogAsset, Error, "Audio Clip not found: {}", path)
			return nullptr;
		}

		return s_Instance->m_AudioClipStore[path];
	}

	

	Ref<Asset> AssetSubsystem::GetSceneAsset(const std::string& _path)
	{
		// We Should validate the path first
        std::string path = FileSubsystem::FormatRelativePath(_path);
		std::ranges::transform(path, path.begin(), ::tolower);
		
		// Check registered assets
		for (const auto& asset : s_Instance->m_SceneStore)
		{
			std::string assetPath = FileSubsystem::FormatRelativePath(asset->GetAssetPath());
			std::ranges::transform(assetPath, assetPath.begin(), ::tolower);
			DE_LOG(LogAsset, Trace, "1. Pathhh Scene: {}", path)
			DE_LOG(LogAsset, Trace, "2. Pathhh Scene: {}", asset->GetAssetPath())
			DE_LOG(LogAsset, Trace, assetPath == path)
			if (assetPath == path)
			{
				return asset;
			}
		}

		return nullptr;
	}

	void AssetSubsystem::SetStartupScene(const std::string& _scenePath)
	{
		if (!GetSceneAsset(_scenePath))
		{
			DE_LOG(LogAsset, Error, "Invalid Scene Asset")
			return;
		}

		s_Instance->m_StartupScene = MakeRef<Asset>(FileSubsystem::FormatRelativePath(_scenePath));
		Engine::GetInstance()->m_Config.StartupScenePath = s_Instance->m_StartupScene->GetAssetPath();
	}

	////////////////////////  SHADERS ///////////////////////////////
	Ref<Shader> AssetSubsystem::LoadShader(std::vector<ShaderSource>& _shaders, const std::string& _path)
	{
		// Check if the shader already exists
		if (s_Instance->m_ShaderStore.contains(_path))
		{
			DE_LOG(LogShader, Error, "GLShader already exists: {}", _path)
			return nullptr;
		}

		// Create a new shader
		if (const Ref<Shader> shader = MakeRef<Shader>())
		{
			// Format the shader sources
			for (auto& source : _shaders)
			{
				source.Path = FileSubsystem::FormatPath(source.Path);
				source.FileName = std::filesystem::path(source.Path).filename().string();
				source.Source = FileSubsystem::ReadFile(source.Path);
				source.Type = shader->GetShaderType(source.Source);
			}
			shader->m_ShaderSources = _shaders;

			if (!shader->CompileProgram()) return nullptr;

			s_Instance->m_ShaderStore[_path] = shader;

			DE_LOG(LogShader, Trace, "Shader Loaded: {}", _path)
			
			return shader;
		}

		DE_LOG(LogShader, Error, "Failed to load shader: {}", _path)
		return nullptr;
	}

	bool AssetSubsystem::ReloadShader(const Ref<Shader>& _shader)
	{
		if (!_shader)
		{
			DE_LOG(LogAsset, Error, "Invalid Shader")
			return false;
		}
		
		if (_shader->RecompileProgram()) return false;
		
		DE_LOG(LogAsset, Info, "Shader Recompiled successfully: {}", _shader->GetDirectoryName())
		return true;
	}

	Ref<Shader> AssetSubsystem::GetShader(const std::string& _name)
	{
		if (s_Instance->m_ShaderStore.contains(_name))
		{
			return s_Instance->m_ShaderStore[_name];
		}

		DE_LOG(LogShader, Error, "GLShader not found: {}", _name)
			return nullptr;
	}

	

	////////////////////////  TEXTURES ///////////////////////////////
	Ref<Texture> AssetSubsystem::LoadTexture(const std::string& _path)
	{
		std::string path =FileSubsystem::FormatPath(_path);
		
		// Check it isn't already loaded
		if (s_Instance->m_TextureStore.contains(path))
		{
			DE_LOG(LogAsset, Warn, "Load Texture: A texture name: {} is already loaded", path)
				return s_Instance->m_TextureStore[path];
		}

		Ref<Texture> texture = MakeRef<Texture>(path);

		if (!texture->LoadTexture()) return nullptr;

		s_Instance->m_TextureStore[path] = texture;
		DE_LOG(LogAsset, Trace, "Texture loaded: {}", path)

		return texture;
	}
	
	Ref<Texture> AssetSubsystem::GetTexture(const std::string& _path)
	{
		if (s_Instance->m_TextureStore.contains(_path))
		{
			return s_Instance->m_TextureStore[_path];
		}

		return nullptr;
	}

	////////////////////////  MATERIALS ///////////////////////////////
	Ref<Material> AssetSubsystem::LoadMaterial(const Ref<Asset>& _matAsset)
	{
		if (s_Instance->m_MaterialStore.contains(_matAsset->m_AssetName))
		{
			DE_LOG(LogAsset, Error, "Load Material: A material name: {} is already loaded", _matAsset->m_AssetName)
			return s_Instance->m_MaterialStore[_matAsset->m_AssetName];
		}

		if (Ref<Material> material = MakeRef<Material>(_matAsset))
		{
			s_Instance->m_MaterialStore[_matAsset->m_AssetName] = material;
			DE_LOG(LogAsset, Trace, "Material Loaded: {}", _matAsset->m_AssetName)
			return material;
		}
		
		DE_LOG(LogAsset, Error, "Failed to load material: {}", _matAsset->m_AssetName)
		return nullptr;
	}

	Ref<Material> AssetSubsystem::GetMaterial(const std::string& _path)
	{
		for (const auto& material : s_Instance->m_MaterialStore)
		{
			if (material.second->GetAsset()->GetAssetPath() == _path)
			{
				return material.second;
			}
		}

		return nullptr;
	}

	////////////////////////  MODEL ///////////////////////////////
	bool AssetSubsystem::LoadModel(const std::string& _path)
	{
		if (s_Instance->m_ModelStore.contains(_path))
		{
			DE_LOG(LogAsset, Error, "Load Model: A Model name: {} is already loaded", _path)
				return false;
		}

		const Ref<Model> model = MakeRef<Model>(_path);
		if (!model->LoadModel())
		{
			DE_LOG(LogAsset, Error, "Failed to load model: {}", _path)
			return false;
		}

		for (auto texture : model->m_Textures)
		{
			if (texture)
			{
				if (!texture->GetTextureID())
				{
					texture = GetTexture("DefaultTexture");
				}
			}
		}

		DE_LOG(LogAsset, Trace, "Model Loaded: {}", model->GetAssetName())
			s_Instance->m_ModelStore[_path] = model;

		return true;
	}

	Ref<Model> AssetSubsystem::GetModel(const std::string& _name)
	{
		if (const std::string path = FileSubsystem::FormatPath(_name); s_Instance->m_ModelStore.contains(path))
			return s_Instance->m_ModelStore[path];

		return nullptr;
	}
}