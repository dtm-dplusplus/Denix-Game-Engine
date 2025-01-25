#pragma once
#include <string>
#include "Denix/Core/Log/Logger.h"

namespace Denix
{
    struct AssetInit
    {
        // Constructors
        AssetInit() : AssetPath{ "Asset" } {}
        AssetInit(std::string _path) : AssetPath{ std::move(_path) }{}
        
        std::string AssetPath;
    };

    
class Asset
{
public:
    Asset() = default;
    virtual ~Asset() = default;
    
   Asset(const AssetInit& _assetInit);

    bool RenameAsset(const std::string& _newName)
    {
        // Add check for invalid characters
        static std::string invalidChars = "\\/:*?\"<>|";

        for (const auto& c : invalidChars)
        {
            if (_newName.find(c) != std::string::npos)
            {
                DE_LOG(LogAsset, Error, "Invalid character in asset name: {0}", c)
                return false;
            }
        }

        // Add check for empty string or same name
        if( _newName.empty() || _newName == m_AssetName)
        {
            DE_LOG (LogAsset, Error, "Invalid asset name: {0}", _newName)
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
    std::string GetRelativePath() const
    {
        return m_AssetRelativePath;
    }


    std::string GetAbsolutePath() const
    {
        return m_AssetAbsolutePath;
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

    std::string GetDirectoryName() const
    {
        return m_DirectoryName;
    }
    
private:
    // Relative Path
    std::string m_AssetRelativePath;

    std::string m_AssetAbsolutePath;
    
    // Asset name without extension
    std::string m_AssetName;

    // Asset file name with extension
    std::string m_AssetFileName;

    // Asset file extension
    std::string m_AssetExtension;
    
    // Asset directory
    std::string m_AssetDirectory;

    std::string m_DirectoryName;

    friend class AssetSubsystem;
};
}
