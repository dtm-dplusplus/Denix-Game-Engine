/**
 * @file FileSubsystem.h
 * @brief Declaration of the FileSubsystem class, responsible for file management operations.
 */

#pragma once

#include "Denix/Core/Subsystem.h"
#include "Denix/Core.h"

namespace Denix
{
    /**
     * @class FileSubsystem
     * @brief Manages file system operations such as reading, writing, and path formatting.
     */
    class FileSubsystem : public Subsystem<FileSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        FileSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~FileSubsystem() override = default;

        FileSubsystem(const FileSubsystem& _other) = delete;
        FileSubsystem(FileSubsystem&& _other) noexcept = delete;
        FileSubsystem& operator=(const FileSubsystem& _other) = delete;
        FileSubsystem& operator=(FileSubsystem&& _other) noexcept = delete;

        /**
         * @brief Gets the root directory of the project.
         * @return The project root path.
         */
        static std::string GetProjectRoot() { return s_Instance->m_ProjectRoot; }

        /**
         * @brief Gets the root directory of the content.
         * @return The content root path.
         */
        static std::string GetContentRoot() { return s_Instance->m_ContentRoot; }

        /**
         * @brief Gets the root directory of the engine's content.
         * @return The engine content root path.
         */
        static std::string GetEngineContentRoot() { return s_Instance->m_EngineContentRoot; }

        /**
         * @brief Copies a file from one location to another.
         * @param _oldPath The source file path.
         * @param _newPath The destination file path.
         * @return True if the operation was successful, false otherwise.
         */
        static bool CopyFileDE(const std::string& _oldPath, const std::string& _newPath);

        /**
         * @brief Reads the contents of a file.
         * @param _path The file path.
         * @return The file contents as a string.
         */
        static std::string ReadFile(const std::string& _path);

        /**
         * @brief Writes data to a file.
         * @param _path The file path.
         * @param _data The data to be written.
         * @return True if the operation was successful, false otherwise.
         */
        static bool WriteFile(const std::string& _path, std::string_view _data);

        /**
         * @brief Checks if a file exists.
         * @param _path The file path.
         * @return True if the file exists, false otherwise.
         */
        static bool FileExists(const std::string& _path);

        /**
         * @brief Checks if a directory exists.
         * @param _path The directory path.
         * @return True if the directory exists, false otherwise.
         */
        static bool DirectoryExists(const std::string& _path);

        /**
         * @brief Creates a new directory.
         * @param _path The directory path.
         * @return True if the operation was successful, false otherwise.
         */
        static bool CreateDirectoryDE(const std::string& _path);

        /**
         * @brief Formats a given file path.
         * @param _path The original file path.
         * @return The formatted file path.
         */
        static std::string FormatPath(const std::string& _path);

        /**
         * @brief Formats a relative file path.
         * @param _path The original file path.
         * @return The formatted relative file path.
         */
        static std::string FormatRelativePath(const std::string& _path);

        /**
         * @brief Checks if a given path is absolute.
         * @param _path The file path.
         * @return True if the path is absolute, false otherwise.
         */
        static bool IsAbsolute(const std::string& _path);

    private:
        /**
         * @brief Initializes the file subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the file subsystem.
         */
        void Deinitialize() override;

        std::string m_ProjectRoot; /**< Root directory of the project. */
        std::string m_ProjectFile; /**< Project file path. */
        std::string m_BinaryRoot; /**< Root directory for binary files. */
        std::string m_ContentRoot; /**< Root directory of content files. */
        std::string m_EngineContentRoot; /**< Root directory for engine content files. */

        friend class Engine;
    };
}
