#include <switch.h>
#include <deko3d.h>
#include <cstdio>

static DkDevice s_device;
static DkQueue s_queue;
static DkSwapchain s_swapchain;
static DkCmdBuf s_cmdbuf;
static DkMemBlock s_memBlock;
static DkImage s_framebuffers[2];
static DkMemBlock s_fbMemBlocks[2];
static bool s_inited = false;
static int s_width = 1280;
static int s_height = 720;

bool Renderer_Init(int w, int h) {
    s_width = w; s_height = h;
    s_device = dkDeviceCreate(NULL);
    if(!s_device) return false;
    
    s_queue = dkQueueCreate(s_device);
    if(!s_queue) return false;
    
    DkMemBlockMaker maker;
    dkMemBlockMakerDefaults(&maker, s_device, 0);
    s_memBlock = dkMemBlockCreate(&maker);
    
    DkCmdBufMaker cmdMaker;
    dkCmdBufMakerDefaults(&cmdMaker, s_device);
    s_cmdbuf = dkCmdBufCreate(&cmdMaker);
    
    DkSwapchainMaker scMaker;
    dkSwapchainMakerDefaults(&scMaker, s_device, nwindowGetDefault(), s_width, s_height, 2);
    s_swapchain = dkSwapchainCreate(&scMaker);
    
    // Create framebuffers
    for(int i=0;i<2;i++) {
        DkImageLayoutMaker lm;
        dkImageLayoutMakerDefaults(&lm, s_device);
        lm.flags = DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression;
        lm.format = DkImageFormat_RGBA8_Unorm;
        lm.dimensions[0] = s_width;
        lm.dimensions[1] = s_height;
        
        DkImageLayout layout;
        dkImageLayoutInitialize(&layout, &lm);
        
        dkMemBlockMakerDefaults(&maker, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image);
        maker.size = layout.storageSize;
        s_fbMemBlocks[i] = dkMemBlockCreate(&maker);
        
        dkImageCreate(&s_framebuffers[i], &layout, s_fbMemBlocks[i], 0);
    }
    
    s_inited = true;
    return true;
}

void Renderer_BeginFrame() {
    if(!s_inited) return;
    dkCmdBufClearColor(s_cmdbuf, 0, DkColorFormat_RGBA8_Unorm, 0x000000FF); // hitam pekat
}

void Renderer_EndFrame() {
    if(!s_inited) return;
    int slot = dkSwapchainAcquireImage(s_swapchain);
    if(slot < 0) return;
    
    DkImageView view;
    dkImageViewDefaults(&view, &s_framebuffers[slot]);
    
    dkCmdBufBindRenderTarget(s_cmdbuf, &view, NULL);
    dkCmdBufClearColor(s_cmdbuf, 0, DkColorFormat_RGBA8_Unorm, 0x000000FF);
    
    dkQueueSubmitCommands(s_queue, s_cmdbuf);
    dkQueuePresentImage(s_queue, s_swapchain, slot);
}

void Renderer_Shutdown() {
    if(!s_inited) return;
    dkQueueWaitIdle(s_queue);
    dkSwapchainDestroy(s_swapchain);
    dkCmdBufDestroy(s_cmdbuf);
    dkQueueDestroy(s_queue);
    dkDeviceDestroy(s_device);
    s_inited = false;
}
