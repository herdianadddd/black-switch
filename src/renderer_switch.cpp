#include <switch.h>

static Framebuffer s_fb;
static bool s_inited = false;

bool Renderer_Init(int w, int h) {
    if (s_inited) return true;
    NWindow *win = nwindowGetDefault();
    if (!win) return false;
    
    // Ini API stabil yang dipakai semua contoh Nintendo
    // PIXEL_FORMAT_RGBA_8888 = 0
    Result rc = framebufferCreate(&s_fb, win, (u32)w, (u32)h, PIXEL_FORMAT_RGBA_8888, 2);
    if (R_FAILED(rc)) return false;
    
    rc = framebufferMakeLinear(&s_fb);
    if (R_FAILED(rc)) {
        framebufferClose(&s_fb);
        return false;
    }
    s_inited = true;
    return true;
}

void Renderer_BeginFrame() {
    if (!s_inited) return;
    // dapet pointer buat nulis pixel
    u32 stride;
    u32 *buf = (u32*)framebufferBegin(&s_fb, &stride);
    if (buf) {
        // Isi hitam (0xFF000000 = hitam opaque)
        // 1280*720 = 921600 pixel
        for (int i = 0; i < 1280*720; i++) {
            buf[i] = 0xFF000000;
        }
    }
}

void Renderer_EndFrame() {
    if (!s_inited) return;
    framebufferEnd(&s_fb);
}

void Renderer_Shutdown() {
    if (!s_inited) return;
    framebufferClose(&s_fb);
    s_inited = false;
}
