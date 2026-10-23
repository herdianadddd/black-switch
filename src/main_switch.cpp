#include <switch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "renderer_switch.h"
#include "wine_launcher.h"

enum Screen { LAUNCHER, SETTINGS };

struct ContainerConfig {
    int box64Preset = 0; // 0 Perf, 1 Compat, 2 Battery
    int wineVersion = 2; // 0 8.0, 1 9.0, 2 11.0
    int driver = 0; // 0 Nouveau, 1 Turnip
    int res = 0; // 0 720p, 1 1080p
};

int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);

    const char* gamesPath = "sdmc:/switch/winlator-nx/games";
    mkdir("sdmc:/switch", 0777);
    mkdir("sdmc:/switch/winlator-nx", 0777);
    mkdir(gamesPath, 0777);

    std::vector<std::string> games;
    DIR* d = opendir(gamesPath);
    if(d){
        dirent* ent;
        while((ent = readdir(d))){
            std::string n = ent->d_name;
            if(n.size()>4 && (n.substr(n.size()-4)==".exe" || n.substr(n.size()-4)==".EXE"))
                games.push_back(n);
        }
        closedir(d);
    }
    if(games.empty()){
        games.push_back("quake3.exe");
        games.push_back("openttd.exe");
        games.push_back("nfsu2.exe");
        games.push_back("gta_sa.exe");
    }

    int sel = 0;
    int settingsSel = 0;
    Screen screen = LAUNCHER;
    ContainerConfig cfg;
    bool launching = false;
    int timer = 0;

    const char* box64Names[] = {"Performance", "Compatibility", "Battery Saver"};
    const char* wineNames[] = {"Wine 8.0", "Wine 9.0", "Wine 11.0 NX"};
    const char* driverNames[] = {"Nouveau 20.1", "Turnip (Vulkan)"};
    const char* resNames[] = {"1280x720", "1920x1080"};

    while(appletMainLoop()){
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if(kDown & HidNpadButton_Plus) break;

        if(screen==LAUNCHER){
            if(kDown & HidNpadButton_X){ screen = SETTINGS; settingsSel=0; }
            if(!launching){
                if(kDown & HidNpadButton_AnyUp && sel>0) sel--;
                if(kDown & HidNpadButton_AnyDown && sel < (int)games.size()-1) sel++;
                if(kDown & HidNpadButton_A){ launching=true; timer=0; }
            }
        } else { // SETTINGS
            if(kDown & HidNpadButton_B) screen = LAUNCHER;
            if(kDown & HidNpadButton_AnyUp && settingsSel>0) settingsSel--;
            if(kDown & HidNpadButton_AnyDown && settingsSel<3) settingsSel++;
            if(kDown & HidNpadButton_AnyLeft || kDown & HidNpadButton_AnyRight){
                if(settingsSel==0) cfg.box64Preset = (cfg.box64Preset+1)%3;
                if(settingsSel==1) cfg.wineVersion = (cfg.wineVersion+1)%3;
                if(settingsSel==2) cfg.driver = (cfg.driver+1)%2;
                if(settingsSel==3) cfg.res = (cfg.res+1)%2;
            }
        }

        Renderer_BeginFrame();
        Renderer_DrawRect(0,0,1280,720,0xFF0A0A0A);

        if(screen==LAUNCHER){
            Renderer_DrawRect(0,0,1280,85,0xFF151515);
            Renderer_DrawRect(30,20,200,45,0xFF2222FF); // WINLATOR NX
            Renderer_DrawRect(1050,25,150,35,0xFF333333); // [X] Settings

            for(int i=0;i<(int)games.size();i++){
                int y = 120 + i*85;
                u32 bg = (i==sel) ? 0xFF2A2AFF : 0xFF1E1E1E;
                u32 bar = (i==sel) ? 0xFFFFFFFF : 0xFF444444;
                Renderer_DrawRect(40,y,1200,75,bg);
                Renderer_DrawRect(40,y,8,75,bar);
                Renderer_DrawRect(70,y+20,30,35,bar);
            }
            if(launching){
                timer++;
                Renderer_DrawRect(0,0,1280,720,0x88000000);
                Renderer_DrawRect(390,300,500,140,0xFF101010);
                Renderer_DrawRect(390,300,500,8,0xFF2A2AFF);
                if(timer>120) launching=false;
            }
        } else {
            // SETTINGS SCREEN ala Winlator Android
            Renderer_DrawRect(0,0,1280,85,0xFF151515);
            Renderer_DrawRect(30,20,300,45,0xFF2222FF); // CONTAINER SETTINGS
            
            // 4 rows settings
            for(int i=0;i<4;i++){
                int y = 130 + i*110;
                u32 bg = (i==settingsSel) ? 0xFF2A2AFF : 0xFF1E1E1E;
                Renderer_DrawRect(40,y,1200,95,bg);
                Renderer_DrawRect(60,y+15,400,20,0xFF888888); // label
                Renderer_DrawRect(60,y+50,250,30,0xFFFFFFFF); // value box
            }
            Renderer_DrawRect(40,620,1200,40,0xFF333333); // hint [B] Back  [Left/Right] Change
        }

        Renderer_EndFrame();
    }
    Renderer_Shutdown();
    return 0;
}
