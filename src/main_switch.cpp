#include <switch.h>
#include "renderer_switch.h"
int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);
    while(appletMainLoop()){
        padUpdate(&pad);
        if(padGetButtonsDown(&pad) & HidNpadButton_Plus) break;
        Renderer_BeginFrame();
        Renderer_EndFrame();
    }
    Renderer_Shutdown();
    return 0;
}
