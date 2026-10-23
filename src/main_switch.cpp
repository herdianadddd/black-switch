#include <switch.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    printf("Black Switch OK!\n");
    printf("Press PLUS 5 detik auto exit\n");
    
    int frames = 0;
    while(appletMainLoop()) {
        consoleUpdate(NULL);
        frames++;
        if(frames > 300) break;
    }
    
    consoleExit(NULL);
    return 0;
}
