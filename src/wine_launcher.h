#pragma once
#include <string>

struct Box64Config {
    int box64Preset = 0; // 0=SAFE,1=PERF,2=COMPAT
    int wineVersion = 2; // 0=8.0,1=9.0,2=10.0
    int driver = 0;      // 0=VULKAN,1=OPENGL
    int res = 0;         // 0=720p,1=1080p
};

void WineLauncher_Init();
void WineLauncher_Launch(const std::string& exePath, const Box64Config& cfg);
