#include <switch.h>
#include "renderer_switch.h"

static Framebuffer s_fb;
static bool s_inited = false;

bool Renderer_Init(int w, int h){
    NWindow *win = nwindowGetDefault();
    if(!win) return false;
    Result rc = framebufferCreate(&s_fb, win, (u32)w, (u32)h, PIXEL_FORMAT_RGBA_8888, 2);
    if(R_FAILED(rc)) return false;
    rc = framebufferMakeLinear(&s_fb);
    if(R_FAILED(rc)){framebufferClose(&s_fb);return false;}
    s_inited=true;
    return true;
}

void Renderer_BeginFrame(){
    if(!s_inited) return;
    u32 stride;
    u32 *buf = (u32*)framebufferBegin(&s_fb, &stride);
    if(!buf) return;

    // Background merah
    for(int i=0;i<1280*720;i++) buf[i]=0xFFFF0000;

    // Kotak hitam di tengah 400x200
    // Layar 1280x720 -> tengah x=440-840, y=260-460
    for(int y=260; y<460; y++){
        for(int x=440; x<840; x++){
            buf[y*1280 + x] = 0xFF000000;
        }
    }
}

void Renderer_EndFrame(){
    if(s_inited) framebufferEnd(&s_fb);
}

void Renderer_Shutdown(){
    if(s_inited){framebufferClose(&s_fb);s_inited=false;}
}
