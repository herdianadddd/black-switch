#include "renderer_switch.h"
static int g_w=1280, g_h=720;
void Renderer_Init(int w, int h){ g_w=w; g_h=h; }
void Renderer_Shutdown(){}
void Renderer_BeginFrame(){}
void Renderer_EndFrame(){}
void Renderer_DrawRect(int x,int y,int w,int h,u32 col){ (void)x;(void)y;(void)w;(void)h;(void)col; }
