#include "overlay.h"

int main() {
    HINSTANCE hInst = GetModuleHandle(nullptr);
    if (!OverlayInit(hInst)) return 1;
    OverlayLoop();
    OverlayShutdown();
    return 0;
}
