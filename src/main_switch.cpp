#include <switch.h>
#include <dirent.h>
#include <string.h>
#include <vector>
#include <string>
#include "renderer_switch.h"

int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);

    // Folder games Winlator NX
    const char* gamesPath = "sdmc:/switch/winlator-nx/games";
    mkdir("sdmc:/switch", 0777);
    mkdir("sdmc:/switch/winlator-nx", 0777);
    mkdir(gamesPath, 0777);

    std::vector<std::string> games;
    DIR* d = opendir(gamesPath);
    if(d){
        struct dirent* ent;
        while((ent = readdir(d))){
            std::string n = ent->d_name;
            if(n.find(".exe") != std::string::npos || n.find(".EXE") != std::string::npos)
                games.push_back(n);
        }
        closedir(d);
    }
    if(games.empty()){
        games.push_back("quake3.exe (demo)");
        games.push_back("openttd.exe (demo)");
        games.push_back("nfsu2.exe (demo)");
    }

    int selected = 0;
    bool launching = false;
    int launchTimer = 0;

    while(appletMainLoop()){
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if(kDown & HidNpadButton_Plus) break;

        if(!launching){
            if(kDown & HidNpadButton_AnyUp) { if(selected>0) selected--; }
            if(kDown & HidNpadButton_AnyDown) { if(selected < (int)games.size()-1) selected++; }
            if(kDown & HidNpadButton_A){ launching = true; launchTimer = 0; }
        }

        Renderer_BeginFrame();
        // Background Black Switch theme hitam pekat
        Renderer_DrawRect(0,0,1280,720,0xFF0A0A0A);
        
        // Header
        Renderer_DrawRect(0,0,1280,80,0xFF1A1A1A);
        // Title bar merah dikit biar keren - tulisan WINLATOR NX
        // Kotak putih buat header text placeholder
        
        // List games
        for(int i=0;i<(int)games.size();i++){
            int y = 120 + i*80;
            u32 color = (i==selected) ? 0xFFFFFFFF : 0xFF333333; // selected putih
            u32 bg = (i==selected) ? 0xFF2222FF : 0xFF1A1A1A; // selected biru
            Renderer_DrawRect(50, y, 1180, 70, bg);
            Renderer_DrawRect(60, y+10, 20, 50, color); // icon kecil
        }

        if(launching){
            launchTimer++;
            Renderer_DrawRect(0,0,1280,720,0xAA000000);
            Renderer_DrawRect(440, 300, 400, 120, 0xFF2222FF); // popup launch
            // nanti disini panggil wine via box64:
            // box64 wine sdmc:/switch/winlator-nx/games/<selected>
            if(launchTimer > 120) launching = false;
        }

        Renderer_EndFrame();
    }
    Renderer_Shutdown();
    return 0;
}
