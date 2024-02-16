#include <Windows.h>
#include "ntstatus.h"
#include <bcrypt.h>
#pragma comment (lib, "Bcrypt.lib")

void start(void);

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
) {
    start();
    return 0;
}

void trigger_breakpoint() {
    DebugBreak();
}