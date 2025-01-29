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
