#include <switch.h>
#include <cstdio>

bool Renderer_Init(int w, int h);
void Renderer_BeginFrame();
void Renderer_EndFrame();
void Renderer_Shutdown();

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);
    
    Renderer_Init(1280, 720);
    printf("Black Switch - Engine OK\n");
    printf("Press PLUS to exit\n");
    
    while(appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if(kDown & HidNpadButton_Plus) break;
        
        Renderer_BeginFrame();
        Renderer_EndFrame();
        consoleUpdate(NULL);
    }
    
    Renderer_Shutdown();
    consoleExit(NULL);
    return 0;
}
