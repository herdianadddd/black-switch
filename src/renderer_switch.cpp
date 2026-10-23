#include <switch.h>
#include <deko3d.h>

static bool s_inited = false;
static DkDevice s_device = NULL;
static DkQueue s_queue = NULL;

bool Renderer_Init(int w, int h) {
    (void)w; (void)h;
    s_device = dkDeviceCreate(NULL);
    if(!s_device) return false;
    s_queue = dkQueueCreate(s_device);
    if(!s_queue) return false;
    s_inited = true;
    return true;
}

void Renderer_BeginFrame() {
    if(!s_inited) return;
    // nanti kita isi clear hitam di sini
}

void Renderer_EndFrame() {
    if(!s_inited) return;
    // nanti present di sini
}

void Renderer_Shutdown() {
    if(!s_inited) return;
    dkQueueWaitIdle(s_queue);
    dkQueueDestroy(s_queue);
    dkDeviceDestroy(s_device);
    s_inited = false;
}
