#pragma once
#include <switch.h>

void Renderer_Init(int w, int h);
void Renderer_Shutdown();
void Renderer_BeginFrame();
void Renderer_EndFrame();
void Renderer_DrawRect(int x, int y, int w, int h, u32 col);
