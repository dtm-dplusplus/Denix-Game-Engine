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
    
    Asset(const std::string& _assetPath);

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

    
    /**
     * 
     * @return Asset name without extension
     */
    std::string GetAssetName() const
    {
        return m_AssetName;
    }

    /**
     * 
     * @return Asset file name with extension
     */
    std::string GetAssetFileName() const
    {
        return m_AssetFileName;
    }
    
    /**
     * 
     * @return Asset file path 
     */
    std::string GetAssetPath() const
    {
        return m_AssetPath;
    }

    /**
     * 
     * @return Asset file extension
     */
    std::string GetAssetExtension() const
    {
        return m_AssetExtension;
    }

    
    /**
     * 
     * @return Asset directory
     */
    std::string GetAssetDirectory() const
    {
        return m_AssetDirectory;
    }

    
protected:
    // Asset name without extension
    std::string m_AssetName;

    // Asset file name with extension
    std::string m_AssetFileName;

    // Asset file extension
    std::string m_AssetExtension;
    
    // Asset file path
    std::string m_AssetPath;

    // Asset directory
    std::string m_AssetDirectory;

    friend class ResourceSubsystem;
};
}
