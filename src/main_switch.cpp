#include <switch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include "renderer_switch.h"
#include "wine_launcher.h"

enum Screen { LAUNCHER, SETTINGS };
struct ContainerConfig { int box64Preset=0; int wineVersion=2; int driver=0; int res=0; char lastGame[64]={0}; };
void SaveConfig(ContainerConfig &c){ FILE* f=fopen("sdmc:/switch/winlator-nx/container.conf","wb"); if(f){ fwrite(&c,sizeof(c),1,f); fclose(f);} }
void LoadConfig(ContainerConfig &c){ FILE* f=fopen("sdmc:/switch/winlator-nx/container.conf","rb"); if(f){ fread(&c,sizeof(c),1,f); fclose(f);} }

const char* GetDriverName(int d){ return d==0?"Vulkan":"OpenGL"; }
const char* GetPresetName(int p){ if(p==0) return "Safe"; if(p==1) return "Perf"; return "Compat"; }
const char* GetWineName(int w){ if(w==0) return "8.0"; if(w==1) return "9.0"; return "10.0"; }

int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);
    mkdir("sdmc:/switch",0777); mkdir("sdmc:/switch/winlator-nx",0777); mkdir("sdmc:/switch/winlator-nx/games",0777);
    WineLauncher_Init();
    ContainerConfig cfg; LoadConfig(cfg);
    std::vector<std::string> games; DIR* d=opendir("sdmc:/switch/winlator-nx/games");
    if(d){ dirent* ent; while((ent=readdir(d))){ std::string n=ent->d_name; if(n.size()>4 && n.substr(n.size()-4)==".exe") games.push_back(n);} closedir(d);}
    if(games.empty()){ games.push_back("quake3.exe"); games.push_back("openttd.exe"); games.push_back("nfsu2.exe"); games.push_back("gta_sa.exe");}
    if(strlen(cfg.lastGame)>0){ for(int i=0;i<(int)games.size();i++) if(games[i]==cfg.lastGame){ std::swap(games[0], games[i]); break; } }
    int sel=0, settingsSel=0; Screen screen=LAUNCHER; bool launching=false; int timer=0; int frameCnt=0;
    while(appletMainLoop()){
        padUpdate(&pad); u64 kDown=padGetButtonsDown(&pad); frameCnt++;
        if(kDown & HidNpadButton_Plus){ SaveConfig(cfg); break; }
        if(screen==LAUNCHER){
            if(kDown & HidNpadButton_X){ screen=SETTINGS; settingsSel=0; }
            if(!launching){ if(kDown & HidNpadButton_AnyUp && sel>0) sel--; if(kDown & HidNpadButton_AnyDown && sel<(int)games.size()-1) sel++; if(kDown & HidNpadButton_A){ launching=true; timer=0; Box64Config bc{cfg.box64Preset,cfg.wineVersion,cfg.driver,cfg.res}; std::string full="sdmc:/switch/winlator-nx/games/"+games[sel]; WineLauncher_Launch(full,bc); strncpy(cfg.lastGame, games[sel].c_str(), 63); cfg.lastGame[63]=0; SaveConfig(cfg); } }
        }else{
            if(kDown & HidNpadButton_B){ SaveConfig(cfg); screen=LAUNCHER; }
            if(kDown & HidNpadButton_AnyUp && settingsSel>0) settingsSel--;
            if(kDown & HidNpadButton_AnyDown && settingsSel<3) settingsSel++;
            if(kDown & HidNpadButton_AnyLeft || kDown & HidNpadButton_AnyRight || kDown & HidNpadButton_A){ if(settingsSel==0) cfg.box64Preset=(cfg.box64Preset+1)%3; if(settingsSel==1) cfg.wineVersion=(cfg.wineVersion+1)%3; if(settingsSel==2) cfg.driver=(cfg.driver+1)%2; if(settingsSel==3) cfg.res=(cfg.res+1)%2; }
        }
        Renderer_BeginFrame(); Renderer_DrawRect(0,0,1280,720,0xFF0A0A0A);
        if(screen==LAUNCHER){
            Renderer_DrawRect(0,0,1280,85,0xFF151515); 
            // Title
            Renderer_DrawRect(30,20,200,45,0xFF2222FF);
            // PERFORMANCE HUD - Top Right (Winlator style)
            Renderer_DrawRect(750
