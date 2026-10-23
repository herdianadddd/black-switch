#include <switch.h>

bool Renderer_Init(int w, int h){(void)w;(void)h;return true;}
void Renderer_BeginFrame(){}
void Renderer_EndFrame(){}
void Renderer_Shutdown(){}

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    printf("Black Switch OK!\n");
    printf("Press PLUS - 5 detik auto exit\n");
    
    int frames = 0;
    while(appletMainLoop()) {
        consoleUpdate(NULL);
        frames++;
        if(frames > 300) break; // auto exit 5 detik biar ga hang
    }
    
    consoleExit(NULL);
    return 0;
}
