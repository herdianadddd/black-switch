#include <switch.h>
#include "renderer_switch.h"

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    if (!Renderer_Init(1280, 720)) {
        consolePrint("Renderer fail!\n");
        consoleUpdate(NULL);
        svcSleepThread(2000000000);
        consoleExit(NULL);
        return 0;
    }

    while(appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break; // Pencet + keluar
        if (kDown & HidNpadButton_B) break;    // B juga keluar

        Renderer_BeginFrame();
        // nanti di sini draw game lu
        Renderer_EndFrame();
    }

    Renderer_Shutdown();
    consoleExit(NULL);
    return 0;
}
