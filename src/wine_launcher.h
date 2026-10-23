#pragma once
#include <string>
#include <stdio.h>

struct Box64Config { int preset=0; int wine=2; int driver=0; int res=0; };

inline const char* GetBox64PresetStr(int p){
    if(p==0) return "BOX64_DYNAREC_STRONG=1 BOX64_DYNAREC_BIGBLOCK=3";
    if(p==1) return "BOX64_DYNAREC_STRONG=2 BOX64_DYNAREC_SAFEFLAGS=1";
    return "BOX64_DYNAREC_STRONG=0";
}
inline const char* GetWineVersionStr(int w){
    if(w==0) return "sdmc:/switch/winlator-nx/wine-8.0";
    if(w==1) return "sdmc:/switch/winlator-nx/wine-9.0";
    return "sdmc:/switch/winlator-nx/wine-10.0";
}

inline bool WineLauncher_Init(){ return true; }

inline std::string WineLauncher_BuildCommand(std::string exePath, Box64Config cfg){
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "%s %s/bin/wine %s", GetBox64PresetStr(cfg.preset), GetWineVersionStr(cfg.wine), exePath.c_str());
    return std::string(cmd);
}

inline bool WineLauncher_Launch(std::string exePath, Box64Config cfg){
    std::string cmd = WineLauncher_BuildCommand(exePath, cfg);
    printf("[WINLATOR-NX] Launch: %s\n", cmd.c_str());
    // Real Switch will exec box64 here. For now log to sdmc:/switch/winlator-nx/last_launch.log
    FILE* f = fopen("sdmc:/switch/winlator-nx/last_launch.log","w");
    if(f){ fprintf(f, "%s\n", cmd.c_str()); fclose(f); }
    return true;
}
