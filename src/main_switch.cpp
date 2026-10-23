#include <switch.h>
#include "renderer_switch.h"

int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);

    int px = 540, py = 310; // posisi awal tengah
    int speed = 5;

    while(appletMainLoop()){
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if(kDown & HidNpadButton_Plus) break;

        HidAnalogStickState stick = padGetStickPos(&pad, 0);
        if(stick.x > 3000) px += speed;
        if(stick.x < -3000) px -= speed;
        if(stick.y > 3000) py -= speed; // y kebalik
        if(stick.y < -3000) py += speed;

        Renderer_BeginFrame();
        Renderer_DrawRect(px, py, 200, 100, 0xFF000000); // kotak hitam bisa jalan
        Renderer_EndFrame();
    }
    Renderer_Shutdown();
    return 0;
}
