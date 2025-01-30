#include "Asset.h"

#include "Denix/Core/File/FileSubsystem.h"

Denix::Asset::Asset(const AssetInit& _assetInit)
{
    m_AssetAbsolutePath = FileSubsystem::FormatPath(_assetInit.AssetPath);
    m_AssetRelativePath = FileSubsystem::FormatRelativePath(_assetInit.AssetPath);
    std::filesystem::path p(m_AssetAbsolutePath);
    m_AssetName = p.stem().string();
    m_AssetFileName = p.filename().string();
    m_AssetExtension = p.extension().string();
    m_AssetDirectory = p.parent_path().string() + "\\";
    m_DirectoryName = p.parent_path().filename().string();
}

/**
* @brief Renames the asset if the new name is valid.
* @param _newName The new name for the asset.
* @return True if renaming was successful, false otherwise.
*/
inline bool Denix::Asset::RenameAsset(const std::string& _newName)
{
    static std::string invalidChars = "\\/:*?\"<>|";

    for (const auto& c : invalidChars)
    {
        if (_newName.find(c) != std::string::npos)
        {
            DE_LOG(LogAsset, Error, "Invalid character in asset name: {0}", c)
                return false;
        }
    }

    if (_newName.empty() || _newName == m_AssetName)
    {
        DE_LOG(LogAsset, Error, "Invalid asset name: {0}", _newName)
            return false;
    }

    m_AssetName = _newName;
    return true;
}
