/**
 * @file Asset.h
 * @brief Defines the Asset and AssetInit structures for asset management
 */

#pragma once
#include <string>
#include "Denix/Core/Log/Logger.h"

namespace Denix
{
    /**
     * @struct AssetInit
     * @brief Structure for initializing an asset with a file path.
     */
    struct AssetInit
    {
        /**
         * @brief Default constructor initializes the asset with a default path.
         */
        AssetInit() : AssetPath{ "Asset" } {}

        /**
         * @brief Constructor initializes the asset with a specified path.
         * @param _path The asset file path.
         */
        AssetInit(std::string _path) : AssetPath{ std::move(_path) } {}

        std::string AssetPath; ///< Path to the asset file.
    };

    /**
     * @class Asset
     * @brief Represents an asset like a texture, model, or sound file.
     */
    class Asset
    {
    public:
        /**
         * @brief Default constructor.
         */
        Asset() = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~Asset() = default;

        /**
         * @brief Constructs an asset using an AssetInit structure.
         * @param _assetInit Initialization data for the asset.
         */
        Asset(const AssetInit& _assetInit);

        /**
         * @brief Renames the asset if the new name is valid.
         * @param _newName The new name for the asset.
         * @return True if renaming was successful, false otherwise.
         */
        bool RenameAsset(const std::string& _newName);

        /**
         * @brief Gets the asset name without extension.
         * @return The asset name.
         */
        std::string GetAssetName() const { return m_AssetName; }

        /**
         * @brief Gets the asset file name with extension.
         * @return The asset file name.
         */
        std::string GetAssetFileName() const { return m_AssetFileName; }

        /**
         * @brief Gets the asset file path relative to the project.
         * @return The relative file path.
         */
        std::string GetRelativePath() const { return m_AssetRelativePath; }

        /**
         * @brief Gets the absolute file path of the asset.
         * @return The absolute file path.
         */
        std::string GetAbsolutePath() const { return m_AssetAbsolutePath; }

        /**
         * @brief Gets the asset file extension.
         * @return The file extension.
         */
        std::string GetAssetExtension() const { return m_AssetExtension; }

        /**
         * @brief Gets the directory where the asset is stored.
         * @return The asset directory.
         */
        std::string GetAssetDirectory() const { return m_AssetDirectory; }

        /**
         * @brief Gets the name of the directory containing the asset.
         * @return The directory name.
         */
        std::string GetDirectoryName() const { return m_DirectoryName; }

    private:
        std::string m_AssetRelativePath; ///< Relative file path.
        std::string m_AssetAbsolutePath; ///< Absolute file path.
        std::string m_AssetName; ///< Asset name without extension.
        std::string m_AssetFileName; ///< File name with extension.
        std::string m_AssetExtension; ///< File extension.
        std::string m_AssetDirectory; ///< Directory of the asset.
        std::string m_DirectoryName; ///< Name of the directory containing the asset.

        friend class AssetSubsystem; ///< Grants AssetSubsystem access to private members.
    };
}