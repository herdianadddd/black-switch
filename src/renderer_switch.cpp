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

    DkDeviceMaker devMaker;
    dkDeviceMakerDefaults(&devMaker);
    s_device = dkDeviceCreate(&devMaker);
    if(!s_device) return false;

    DkQueueMaker qMaker;
    dkQueueMakerDefaults(&qMaker, s_device);
    s_queue = dkQueueCreate(&qMaker);
    if(!s_queue) return false;

    DkMemBlockMaker memMaker;
    dkMemBlockMakerDefaults(&memMaker, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
    memMaker.size = 1*1024*1024;
    s_cmdMem = dkMemBlockCreate(&memMaker);
    if(!s_cmdMem) return false;

    DkCmdBufMaker cmdMaker;
    dkCmdBufMakerDefaults(&cmdMaker, s_device);
    cmdMaker.memBlock = s_cmdMem;
    cmdMaker.offset = 0;
    cmdMaker.size = memMaker.size;
    s_cmdbuf = dkCmdBufCreate(&cmdMaker);
    if(!s_cmdbuf) return false;

    DkSwapchainMaker scMaker;
    dkSwapchainMakerDefaults(&scMaker, s_device, nwindowGetDefault(), w, h, 2);
    s_swapchain = dkSwapchainCreate(&scMaker);
    if(!s_swapchain) return false;

    s_inited = true;
    return true;
}

void Renderer_BeginFrame() {
    if(!s_inited) return;
    dkCmdBufReset(s_cmdbuf);
}

void Renderer_EndFrame() {
    if(!s_inited) return;
    
    int slot = dkSwapchainAcquireImage(s_swapchain);
    if(slot < 0) return;

    DkImage const* fb = dkSwapchainGetImage(s_swapchain, slot);
    DkImageView view;
    dkImageViewDefaults(&view, fb);

    dkCmdBufBindRenderTarget(s_cmdbuf, &view, nullptr);
    dkCmdBufClearColor(s_cmdbuf, 0, DkColorFormat_RGBA8_Unorm, 0x000000FF);

    DkCmdList list = dkCmdBufFinishList(s_cmdbuf);
    dkQueueSubmitCommands(s_queue, list);
    dkQueuePresentImage(s_queue, s_swapchain, slot);
}

void Renderer_Shutdown() {
    if(!s_inited) return;
    dkQueueWaitIdle(s_queue);
    dkSwapchainDestroy(s_swapchain);
    dkCmdBufDestroy(s_cmdbuf);
    dkMemBlockDestroy(s_cmdMem);
    dkQueueDestroy(s_queue);
    dkDeviceDestroy(s_device);
    s_inited = false;
}
