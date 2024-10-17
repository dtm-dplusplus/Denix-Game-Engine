#pragma once
#include <string>

#include "Denix/Core/Logger.h"

namespace Denix
{
class Asset
    
{
public:
    Asset() = default;
    virtual ~Asset() = default;
    
    Asset(const std::string& _assetPath)
    {
        m_AssetPath = _assetPath;
        m_AssetName = _assetPath.substr(_assetPath.find_last_of("/\\") + 1);
        m_AssetExtension = _assetPath.substr(_assetPath.find_last_of(".") + 1);
        m_AssetDirectory = _assetPath.substr(0, _assetPath.find_last_of("/\\"));
    }
    
    bool RenameAsset(const std::string& _newName)
    {
        // Add check for invalid characters
        static std::string invalidChars = "\\/:*?\"<>|";

        for (const auto& c : invalidChars)
        {
            if (_newName.find(c) != std::string::npos)
            {
                DE_LOG(LogResource, Error, "Invalid character in asset name: {0}", c)
                return false;
            }
        }

        // Add check for empty string or same name
        if( _newName.empty() || _newName == m_AssetName)
        {
            DE_LOG (LogResource, Error, "Invalid asset name: {0}", _newName)
            return  false;
        }
            
        // Set new name
        m_AssetName = _newName;
        
        return true;
    }

    std::string GetAssetName() const
    {
        return m_AssetName;
    }

    std::string GetAssetPath() const
    {
        return m_AssetPath;
    }

    std::string GetAssetExtension() const
    {
        return m_AssetExtension;
    }

    std::string GetAssetDirectory() const
    {
        return m_AssetDirectory;
    }

    
protected:
    std::string m_AssetName;
    std::string m_AssetPath;
    std::string m_AssetExtension;
    std::string m_AssetDirectory;
    
};
}
