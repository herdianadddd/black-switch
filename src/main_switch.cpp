#include <switch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "renderer_switch.h"
#include "wine_launcher.h"

int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);

    const char* base = "sdmc:/switch/winlator-nx";
    const char* gamesPath = "sdmc:/switch/winlator-nx/games";
    mkdir("sdmc:/switch", 0777);
    mkdir(base, 0777);
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
    }

    int sel = 0;
    bool launching = false;
    int timer = 0;
    std::string launchName = "";

    while(appletMainLoop()){
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if(kDown & HidNpadButton_Plus) break;
        if(!launching){
            if(kDown & HidNpadButton_AnyUp && sel>0) sel--;
            if(kDown & HidNpadButton_AnyDown && sel < (int)games.size()-1) sel++;
            if(kDown & HidNpadButton_A){
                launching = true; timer = 0; launchName = games[sel];
                WineNX_Launch(std::string(gamesPath)+"/"+launchName);
            }
        }

        Renderer_BeginFrame();
        Renderer_DrawRect(0,0,1280,720,0xFF0A0A0A); // Black Switch hitam pekat
        Renderer_DrawRect(0,0,1280,85,0xFF151515);
        Renderer_DrawRect(30,20,200,45,0xFF2222FF); // logo WINLATOR NX placeholder biru
        
        for(int i=0;i<(int)games.size();i++){
            int y = 120 + i*85;
            u32 bg = (i==sel) ? 0xFF2A2AFF : 0xFF1E1E1E;
            u32 bar = (i==sel) ? 0xFFFFFFFF : 0xFF444444;
            Renderer_DrawRect(40,y,1200,75,bg);
            Renderer_DrawRect(40,y,8,75,bar);
            Renderer_DrawRect(70,y+20,30,35,bar); // icon exe
        }

        if(launching){
            timer++;
            Renderer_DrawRect(0,0,1280,720,0x88000000);
            Renderer_DrawRect(390,300,500,140,0xFF101010);
            Renderer_DrawRect(390,300,500,8,0xFF2A2AFF);
            Renderer_DrawRect(410,360,460,20,0xFFFFFFFF); // text launching
            if(timer>150) launching=false;
        }
        Renderer_EndFrame();
    }
    Renderer_Shutdown();
    return 0;
}
