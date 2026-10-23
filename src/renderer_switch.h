#pragma once
#include <switch.h>
bool Renderer_Init(int w, int h);
void Renderer_BeginFrame();
void Renderer_DrawRect(int x, int y, int w, int h, u32 color);
void Renderer_EndFrame();
void Renderer_Shutdown();
