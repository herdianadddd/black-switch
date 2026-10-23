#include "renderer_switch.h"
static Framebuffer s_fb;
static bool s_inited = false;
static u32* s_buf = nullptr;
static u32 s_stride = 0;

bool Renderer_Init(int w, int h){
    NWindow *win = nwindowGetDefault();
    if(!win) return false;
    Result rc = framebufferCreate(&s_fb, win, (u32)w, (u32)h, PIXEL_FORMAT_RGBA_8888, 2);
    if(R_FAILED(rc)) return false;
    rc = framebufferMakeLinear(&s_fb);
    if(R_FAILED(rc)){framebufferClose(&s_fb);return false;}
    s_inited=true;return true;
}

void Renderer_BeginFrame(){
    if(!s_inited) return;
    s_buf = (u32*)framebufferBegin(&s_fb, &s_stride);
    if(s_buf){
        for(int i=0;i<1280*720;i++) s_buf[i]=0xFFFF0000; // merah
    }
}

void Renderer_DrawRect(int x, int y, int w, int h, u32 color){
    if(!s_buf) return;
    for(int py=y; py<y+h; py++){
        if(py<0||py>=720) continue;
        for(int px=x; px<x+w; px++){
            if(px<0||px>=1280) continue;
            s_buf[py*1280 + px] = color;
        }
    }
}

void Renderer_EndFrame(){if(s_inited){framebufferEnd(&s_fb); s_buf=nullptr;}}
void Renderer_Shutdown(){if(s_inited){framebufferClose(&s_fb);s_inited=false;}}
