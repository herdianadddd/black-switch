#include <switch.h>
#include <deko3d.h>

static DkDevice s_device = nullptr;
static DkQueue s_queue = nullptr;
static DkCmdBuf s_cmdbuf = nullptr;
static DkMemBlock s_cmdMem = nullptr;
static DkSwapchain s_swapchain = nullptr;
static DkFence s_fence;
static bool s_fenced = false;
static bool s_inited = false;

static DkImage s_fbImages[2];
static DkMemBlock s_fbMem[2];
static DkImage const* s_fbImagePtrs[2];

bool Renderer_Init(int w, int h) {
    if(s_inited) return true;
    DkDeviceMaker devMaker; dkDeviceMakerDefaults(&devMaker);
    s_device = dkDeviceCreate(&devMaker);
    if(!s_device) return false;
    DkQueueMaker qMaker; dkQueueMakerDefaults(&qMaker, s_device);
    s_queue = dkQueueCreate(&qMaker);
    if(!s_queue) return false;
    DkMemBlockMaker memMaker;
    dkMemBlockMakerDefaults(&memMaker, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
    memMaker.size = 1*1024*1024;
    s_cmdMem = dkMemBlockCreate(&memMaker);
    if(!s_cmdMem) return false;
    DkCmdBufMaker cmdMaker; dkCmdBufMakerDefaults(&cmdMaker, s_device);
    cmdMaker.memBlock = s_cmdMem;
    cmdMaker.offset = 0; cmdMaker.size = memMaker.size;
    s_cmdbuf = dkCmdBufCreate(&cmdMaker);
    if(!s_cmdbuf) return false;
    DkFenceMaker fenceMaker; dkFenceMakerDefaults(&fenceMaker, s_device);
    s_fence = dkFenceCreate(&fenceMaker);
    DkImageLayout layout;
    DkImageLayoutMaker layoutMaker;
    dkImageLayoutMakerDefaults(&layoutMaker, s_device);
    layoutMaker.flags = DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression;
    layoutMaker.format = DkImageFormat_RGBA8_Unorm;
    layoutMaker.dimensions[0] = w;
    layoutMaker.dimensions[1] = h;
    layoutMaker.dimensions[2] = 1;
    dkImageLayoutInitialize(&layout, &layoutMaker);
    uint64_t fbSize = dkImageLayoutGetSize(&layout);
    uint64_t fbAlign = dkImageLayoutGetAlignment(&layout);
    for(int i=0;i<2;i++) {
        DkMemBlockMaker fbMemMaker;
        dkMemBlockMakerDefaults(&fbMemMaker, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image);
        fbMemMaker.size = (fbSize + fbAlign -1) & ~(fbAlign-1);
        s_fbMem[i] = dkMemBlockCreate(&fbMemMaker);
        DkImageMaker imgMaker;
        dkImageMakerDefaults(&imgMaker, s_device, &layout, s_fbMem[i]);
        dkImageInitialize(&s_fbImages[i], &imgMaker);
        s_fbImagePtrs[i] = &s_fbImages[i];
    }
    DkSwapchainMaker scMaker;
    dkSwapchainMakerDefaults(&scMaker, s_device, nwindowGetDefault(), s_fbImagePtrs, 2);
    s_swapchain = dkSwapchainCreate(&scMaker);
    if(!s_swapchain) return false;
    s_inited = true;
    return true;
}

void Renderer_BeginFrame() {
    if(!s_inited) return;
    if(s_fenced) { dkFenceWait(&s_fence, UINT64_MAX); s_fenced = false; }
}
void Renderer_EndFrame() {
    if(!s_inited) return;
    int slot = 0;
    dkSwapchainAcquireImage(s_swapchain, &slot, &s_fence);
    s_fenced = true;
    DkCmdList list = dkCmdBufFinishList(s_cmdbuf);
    dkQueueSubmitCommands(s_queue, list);
    dkQueuePresentImage(s_queue, s_swapchain, slot);
}
void Renderer_Shutdown() {
    if(!s_inited) return;
    dkQueueWaitIdle(s_queue);
    dkSwapchainDestroy(s_swapchain);
    for(int i=0;i<2;i++) dkMemBlockDestroy(s_fbMem[i]);
    dkCmdBufDestroy(s_cmdbuf);
    dkMemBlockDestroy(s_cmdMem);
    dkQueueDestroy(s_queue);
    dkDeviceDestroy(s_device);
    s_inited = false;
}
