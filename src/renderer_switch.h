#pragma once
#include <switch.h>

void Renderer_Init(int w, int h);
void Renderer_Shutdown();
void Renderer_BeginFrame();
void Renderer_EndFrame();
void Renderer_DrawRect(int x, int y, int w, int h, u32 col);

// TEXT - Simple block version (guaranteed build)
inline void Renderer_DrawText(int x, int y, const char* text, u32 col){
    if(!text) return;
    int cx=x;
    while(*text){
        char c=*text;
        if(c!=' ' && c!='\n'){
            // Draw simple char as block - will show as white blocks for now
            Renderer_DrawRect(cx, y, 12, 16, col);
            Renderer_DrawRect(cx, y, 12, 3, col);
        }
        cx+=14;
        if(cx>1240) break;
        text++;
    }
}
inline void Renderer_DrawTextSmall(int x, int y, const char* text, u32 col){
    if(!text) return;
    int cx=x;
    while(*text){
        char c=*text;
        if(c!=' ' && c!='\n'){
            Renderer_DrawRect(cx, y, 6, 8, col);
        }
        cx+=8;
        if(cx>1240) break;
        text++;
    }
}
