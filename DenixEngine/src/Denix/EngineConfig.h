#pragma once

struct EngineConfig
{
    //  Engine Configuration
    std::string ProjectName;
    std::string StartupScenePath;

    // Input Configuration
    bool MouseMotionLogging = false;
    bool MouseButtonLogging = false;
    bool KeyboardLogging = false;
    bool GamepadLogging = false;
};