#include <switch.h>
#include <deko3d.h>

static DkDevice s_device = nullptr;
static DkQueue s_queue = nullptr;
static DkCmdBuf s_cmdbuf = nullptr;
static DkMemBlock s_cmdMem = nullptr;
static DkSwapchain s_swapchain = nullptr;
static bool s_inited = false;

bool Renderer_Init(int w, int h) {
    if(s_inited) return true;
    DkDeviceMaker dm; dkDeviceMakerDefaults(&dm);
    s_device = dkDeviceCreate(&dm);
    if(!s_device) return false;
    DkQueueMaker qm; dkQueueMakerDefaults(&qm, s_device);
    s_queue = dkQueueCreate(&qm);
    if(!s_queue) return false;
    DkMemBlockMaker mm; dkMemBlockMakerDefaults(&mm, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
    mm.size = 1*1024*1024;
    s_cmdMem = dkMemBlockCreate(&mm);
    if(!s_cmdMem) return false;
    DkCmdBufMaker cm; dkCmdBufMakerDefaults(&cm, s_device);
    cm.memBlock = s_cmdMem; cm.offset=0; cm.size=mm.size;
    s_cmdbuf = dkCmdBufCreate(&cm);
    if(!s_cmdbuf) return false;
    DkSwapchainMaker scm; dkSwapchainMakerDefaults(&scm, s_device, nwindowGetDefault(), w, h, 2);
    s_swapchain = dkSwapchainCreate(&scm);
    if(!s_swapchain) return false;
    s_inited = true;
    return true;
}
void Renderer_BeginFrame(){}
void Renderer_EndFrame(){
    if(!s_inited) return;
    int slot = dkSwapchainAcquireImage(s_swapchain);
    dkQueuePresentImage(s_queue, s_swapchain, slot);
}
void Renderer_Shutdown(){
    if(!s_inited) return;
    dkQueueWaitIdle(s_queue);
    dkSwapchainDestroy(s_swapchain);
    dkCmdBufDestroy(s_cmdbuf);
    dkMemBlockDestroy(s_cmdMem);
    dkQueueDestroy(s_queue);
    dkDeviceDestroy(s_device);
    s_inited=false;
}
