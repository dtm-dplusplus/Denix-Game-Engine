/// @file AudioClio.h
/// @brief Defines the AudioClip class for handling audio playback
#pragma once

#include "AudioPrimitive.h"
#include "Denix/Asset/Asset.h"

namespace Denix
{
    /**
     * @class AudioClip
     * @brief Represents an audio asset that can be played by an AudioSource.
     */
    class AudioClip : public Asset
    {
    public:
        /**
         * @brief Constructs an AudioClip with asset initialization parameters.
         * @param _assetInit Initialization data for the asset.
         */
        AudioClip(const AssetInit& _assetInit);

        /**
         * @brief Destroys the AudioClip.
         */
        ~AudioClip() override;

        /**
         * @brief Gets the OpenAL buffer associated with the audio clip.
         * @return The buffer identifier.
         */
        uint32_t GetBuffer() const { return m_Buffer; }

        /**
         * @brief Gets the WAV file length in bytes.
         * @return The length of the WAV file.
         */
        uint32_t GetWavLength() const { return m_WavLength; }

    private:
        /**
         * @brief Loads the audio clip from file.
         * @return True if the loading was successful, false otherwise.
         */
        bool Load();

        SDL_AudioSpec m_ClipSpec; ///< SDL audio specifications for the clip.
        uint32_t m_Buffer = 0; ///< OpenAL buffer identifier.
        uint32_t m_WavLength = 0; ///< Length of the WAV file in bytes.

        friend class AudioSource;
        friend class AudioSubsystem;
        friend class AssetSubsystem;
    };
}
