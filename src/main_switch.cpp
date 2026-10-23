#include <switch.h>
#include "renderer_switch.h"

int main(int argc,char* argv[]){
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad; padInitializeDefault(&pad);
    Renderer_Init(1280,720);

    // Player hitam
    int px = 540, py = 310, pw = 200, ph = 100;
    int speed = 8;

    // Musuh putih
    int ex = 100, ey = 100, ew = 50, eh = 50;
    int evx = 6, evy = 5;
    bool hit = false;

    while(appletMainLoop()){
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if(kDown & HidNpadButton_Plus) break;

        // Gerak player pakai analog kiri
        HidAnalogStickState stick = padGetStickPos(&pad, 0);
        if(stick.x > 3000) px += speed;
        if(stick.x < -3000) px -= speed;
        if(stick.y > 3000) py -= speed;
        if(stick.y < -3000) py += speed;

        // Batas layar biar ga keluar
        if(px < 0) px = 0;
        if(px > 1280-pw) px = 1280-pw;
        if(py < 0) py = 0;
        if(py > 720-ph) py = 720-ph;

        // Gerak musuh mantul-mantul
        ex += evx;
        ey += evy;
        if(ex <= 0 || ex >= 1280-ew) evx = -evx;
        if(ey <= 0 || ey >= 720-eh) evy = -evy;

        // Cek tabrakan player vs musuh
        hit = (px < ex+ew && px+pw > ex && py < ey+eh && py+ph > ey);

        Renderer_BeginFrame();
        // Kalau ketabrak background jadi merah, kalau engga biru
        if(hit){
            Renderer_DrawRect(0,0,1280,720,0xFF0000FF); // merah flash pas ketabrak
        }
        Renderer_DrawRect(ex, ey, ew, eh, 0xFFFFFFFF); // musuh putih
        Renderer_DrawRect(px, py, pw, ph, 0xFF000000); // player hitam
        Renderer_EndFrame();
    }
    Renderer_Shutdown();
    return 0;
}
