#include <switch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include "renderer_switch.h"
#include "wine_launcher.h"
#include "text_renderer.h"

enum Screen { LAUNCHER, SETTINGS };
struct ContainerConfig { int box64Preset=0; int wineVersion=2; int driver=0; int res=0; char lastGame[64]={0}; };
void SaveConfig(ContainerConfig &c){ FILE* f=fopen("sdmc:/switch/winlator-nx/container.conf","wb"); if(f){ fwrite(&c,sizeof(c),1,f); fclose(f);} }
void LoadConfig(ContainerConfig &c){ FILE* f=fopen("sdmc:/switch/winlator-nx/container.conf","rb"); if(f){ fread(&c,sizeof(c),1,f); fclose(f);} }

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
    int sel=0, settingsSel=0; Screen screen=LAUNCHER; bool launching=false; int timer=0;
    while(appletMainLoop()){
        padUpdate(&pad); u64 kDown=padGetButtonsDown(&pad);
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
        Renderer_DrawRect(750,12,500,60,0xFF1A1A1A); Renderer_DrawRect(750,12,500,3,0xFF2A2AFF);
        u32 colDrv = cfg.driver==0 ? 0xFF22FF22 : 0xFFFFAA22;
        Renderer_DrawRect(770,22,90,40,colDrv); Renderer_DrawRect(870,22,90,40,0xFF4444FF); Renderer_DrawRect(970,22,90,40,0xFFAA44FF); Renderer_DrawRect(1070,22,160,40,0xFF333333);
        Text_DrawSmall(775,27,cfg.driver==0?"VK":"GL",0xFF000000);
        Text_DrawSmall(875,27,cfg.box64Preset==0?"SAFE":cfg.box64Preset==1?"PERF":"COMP",0xFFFFFFFF);
        Text_DrawSmall(975,27,cfg.wineVersion==0?"8.0":cfg.wineVersion==1?"9.0":"10.0",0xFFFFFFFF);
        char cntBuf[32]; snprintf(cntBuf,31,"%d GAMES", (int)games.size()); Text_DrawSmall(1080,27,cntBuf,0xFFFFFFFF);

        if(screen==LAUNCHER){
            Renderer_DrawRect(0,0,1280,85,0xFF151515); 
            Text_Draw(35,25,"WINLATOR-NX",0xFFFFFFFF);
            Text_DrawSmall(35,55,"BLACK SWITCH PORT",0xFF888888);
            for(int i=0;i<(int)games.size();i++){
                int y=120+i*85; u32 bg=(i==sel)?0xFF2A2AFF:0xFF1E1E1E; if(i==0 && strlen(cfg.lastGame)>0) bg=0xFF22AA22;
                Renderer_DrawRect(40,y,1200,75,bg); Renderer_DrawRect(40,y,8,75,(i==sel)?0xFFFFFFFF:0xFF444444);
                Text_Draw(70,y+20,games[i].c_str(),0xFFFFFFFF);
                if(i==0 && strlen(cfg.lastGame)>0) Text_DrawSmall(70,y+50,"[LAST]",0xFFAAFFAA);
            }
            Renderer_DrawRect(0,675,1280,45,0xFF151515);
            Text_DrawSmall(50,690,"A: LAUNCH  X: SETTINGS  +: EXIT",0xFFFFFFFF);
            if(launching){ timer++; Renderer_DrawRect(0,0,1280,720,0x88000000); Renderer_DrawRect(390,300,500,140,0xFF101010); Renderer_DrawRect(390,300,500,8,0xFF2A2AFF); Text_Draw(420,330,"LAUNCHING...",0xFFFFFFFF); Text_DrawSmall(420,370,games[sel].c_str(),0xFFAAAAAA); if(timer>60) launching=false; }
        }else{
            Renderer_DrawRect(0,0,1280,85,0xFF151515); Text_Draw(35,25,"CONTAINER SETTINGS",0xFFFFFFFF);
            const char* names[4]={"BOX64 PRESET","WINE VERSION","GPU DRIVER","RESOLUTION"};
            for(int i=0;i<4;i++){
                int y=130+i*110; u32 bg=(i==settingsSel)?0xFF2A2AFF:0xFF1E1E1E; Renderer_DrawRect(40,y,1200,95,bg);
                Text_Draw(70,y+15,names[i],0xFFFFFFFF);
                const char* val=""; char buf[32];
                if(i==0) val=cfg.box64Preset==0?"SAFE":cfg.box64Preset==1?"PERF":"COMPAT";
                if(i==1){ snprintf(buf,31,"WINE %s", cfg.wineVersion==0?"8.0":cfg.wineVersion==1?"9.0":"10.0"); val=buf; }
                if(i==2) val=cfg.driver==0?"VULKAN":"OPENGL";
                if(i==3) val=cfg.res==0?"720P":"1080P";
                Text_DrawSmall(70,y+50,val,0xFFAAAAAA);
            }
           
