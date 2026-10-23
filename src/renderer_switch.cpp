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
static DkImage const* s_fbPtrs[2];

bool Renderer_Init(int w, int h){
 if(s_inited) return true;
 DkDeviceMaker dm; dkDeviceMakerDefaults(&dm);
 s_device = dkDeviceCreate(&dm);
 if(!s_device) return false;
 DkQueueMaker qm; dkQueueMakerDefaults(&qm, s_device);
 s_queue = dkQueueCreate(&qm);
 if(!s_queue) return false;
 DkMemBlockMaker mm; dkMemBlockMakerDefaults(&mm, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
 mm.size = 1<<20;
 s_cmdMem = dkMemBlockCreate(&mm);
 DkCmdBufMaker cm; dkCmdBufMakerDefaults(&cm, s_device);
 cm.memBlock = s_cmdMem; cm.offset=0; cm.size=mm.size;
 s_cmdbuf = dkCmdBufCreate(&cm);
 DkFenceMaker fm; dkFenceMakerDefaults(&fm, s_device);
 s_fence = dkFenceCreate(&fm);

 DkImageLayoutMaker lm; dkImageLayoutMakerDefaults(&lm, s_device);
 lm.flags = DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression;
 lm.format = DkImageFormat_RGBA8_Unorm;
 lm.dimensions[0] = w; lm.dimensions[1] = h; lm.dimensions[2] = 1;
 DkImageLayout layout; dkImageLayoutInitialize(&layout, &lm);
 uint64_t sz = dkImageLayoutGetSize(&layout);
 uint64_t al = dkImageLayoutGetAlignment(&layout);
 sz = (sz + al -1) & ~(al-1);

 for(int i=0;i<2;i++){
   DkMemBlockMaker fmm; dkMemBlockMakerDefaults(&fmm, s_device, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image);
   fmm.size = sz;
   s_fbMem[i] = dkMemBlockCreate(&fmm);
   dkImageInitialize(&s_fbImages[i], &layout, s_fbMem[i], 0);
   s_fbPtrs[i] = &s_fbImages[i];
 }
 DkSwapchainMaker sm; dkSwapchainMakerDefaults(&sm, s_device, nwindowGetDefault(), s_fbPtrs, 2);
 s_swapchain = dkSwapchainCreate(&sm);
 if(!s_swapchain) return false;
 s_inited = true; return true;
}
void Renderer_BeginFrame(){ if(!s_inited) return; if(s_fenced){ dkFenceWait(&s_fence, UINT64_MAX); s_fenced=false; } }
void Renderer_EndFrame(){ if(!s_inited) return; int slot=0; dkSwapchainAcquireImage(s_swapchain, &slot, &s_fence); s_fenced=true; DkCmdList l = dkCmdBufFinishList(s_cmdbuf); dkQueueSubmitCommands(s_queue, l); dkQueuePresentImage(s_queue, s_swapchain, slot); }
void Renderer_Shutdown(){ if(!s_inited) return; dkQueueWaitIdle(s_queue); dkSwapchainDestroy(s_swapchain); for(int i=0;i<2;i++) dkMemBlockDestroy(s_fbMem[i]); dkCmdBufDestroy(s_cmdbuf); dkMemBlockDestroy(s_cmdMem); dkQueueDestroy(s_queue); dkDeviceDestroy(s_device); s_inited=false; }
