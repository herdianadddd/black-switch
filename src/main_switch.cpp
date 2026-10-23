#include <deko3d.hpp>
#include <switch.h>
#include <cstdio>

static dk::Device s_device;
static dk::Queue s_queue;
static dk::MemBlock s_memBlock;
static dk::CmdBuf s_cmdBuf;
static dk::Image s_framebuffer;
static DkMemBlockMaker memMaker{ s_device, 0 };

bool Renderer_Init(int width, int height) {
    printf("[BLACK-SWITCH] Init deko3d %dx%d\n", width, height);
    
    // Init deko3d
    s_device = dk::DeviceMaker{}.create();
    s_queue = dk::QueueMaker{s_device}.setFlags(DkQueueFlags_Graphics).create();
    s_memBlock = DkMemBlockMaker{s_device, 4*1024*1024}
        .setFlags(DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached)
        .create();
    s_cmdBuf = DkCmdBufMaker{s_device}.create();
    
    printf("[BLACK-SWITCH] deko3d OK\n");
    return true;
}

void Renderer_BeginFrame() {
    s_cmdBuf.clear();
}

void Renderer_EndFrame() {
    s_queue.submitCommands(s_cmdBuf.finishList());
    s_queue.flush();
    // nanti ganti sama nwindow
}

// Stub buat fungsi D3D9 lama biar linker ga error
// Ini yang bikin black-pc 0.2.0 bisa compile tanpa d3d_device.cpp
extern "C" {
    void* Direct3DCreate9() { return (void*)0x1; }
    int D3D_Init() { return 1; }
    void D3D_DrawPrimitive(int type, int start, int count) {
        // TODO: translate ke dk::CmdBuf::draw
    }
    void D3D_SetTexture(int stage, void* tex) {}
    void D3D_SetStreamSource(void* vb) {}
}
