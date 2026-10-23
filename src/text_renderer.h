#pragma once
#include "renderer_switch.h"

inline void Text_Draw(int x, int y, const char* txt, u32 col) {
    if (!txt) return;
    int cx = x;
    while (*txt) {
        char c = *txt;
        if (c!=' ' && c!='\n' && c!='\r') {
            int w=8; int h=(c>='A'&&c<='Z')?14:10;
            if(c=='i'||c=='l'||c=='I') w=4;
            if(c=='m'||c=='w'||c=='M'||c=='W') w=12;
            Renderer_DrawRect(cx,y,w,h,col);
        }
        cx+=10;
        if(c==' ') cx+=4;
        if(cx>1260) break;
        txt++;
    }
}
inline void Text_DrawSmall(int x, int y, const char* txt, u32 col) {
    if (!txt) return;
    int cx=x;
    while(*txt){ if(*txt!=' '&&*txt!='\n') Renderer_DrawRect(cx,y,5,7,col); cx+=7; if(cx>1260) break; txt++; }
}
