#include <switch.h>

extern bool Renderer_Init(int w, int h);
extern void Renderer_BeginFrame();
extern void Renderer_EndFrame();
extern void Renderer_Shutdown();

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    printf("Black Switch - Test Build\n");
    printf("Press PLUS to exit\n");
    
    // Init renderer (stub dulu)
    Renderer_Init(1280, 720);

    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS) break;

        Renderer_BeginFrame();
        // render here later
        Renderer_EndFrame();

        consoleUpdate(NULL);
    }

    Renderer_Shutdown();
    consoleExit(NULL);
    return 0;
}
