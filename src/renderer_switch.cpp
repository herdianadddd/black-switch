#include <switch.h>
#include <deko3d.h>

static DkDevice s_device = nullptr;
static DkQueue s_queue = nullptr;
static bool s_inited = false;

bool Renderer_Init(int w, int h) {
    (void)w; (void)h;
    
    DkDeviceMaker deviceMaker;
    dkDeviceMakerDefaults(&deviceMaker);
    s_device = dkDeviceCreate(&deviceMaker);
    if(!s_device) return false;
    
    DkQueueMaker queueMaker;
    dkQueueMakerDefaults(&queueMaker, s_device);
    s_queue = dkQueueCreate(&queueMaker);
    if(!s_queue) {
        dkDeviceDestroy(s_device);
        return false;
    }
    
    s_inited = true;
    return true;
}

void Renderer_BeginFrame() {
    if(!s_inited) return;
}

void Renderer_EndFrame() {
    if(!s_inited) return;
}

void Renderer_Shutdown() {
    if(!s_inited) return;
    dkQueueWaitIdle(s_queue);
    dkQueueDestroy(s_queue);
    dkDeviceDestroy(s_device);
    s_device = nullptr;
    s_queue = nullptr;
    s_inited = false;
}
