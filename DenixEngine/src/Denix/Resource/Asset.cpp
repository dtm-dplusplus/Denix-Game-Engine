#include "Asset.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Denix
{
    Asset::Asset(const std::string& _assetPath)
    {
        const fs::path path = _assetPath;
        m_AssetName = path.stem().string();
        m_AssetFileName = path.filename().string();
        m_AssetExtension = path.extension().string();
        m_AssetDirectory = path.parent_path().string() + "\\";
        m_AssetPath = _assetPath;
    }
}
