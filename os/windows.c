#include <Windows.h>
#include "ntstatus.h"
#include <bcrypt.h>
#pragma comment (lib, "Bcrypt.lib")

/*
#ifdef slice_bounds_checking
    #define assert(expression) assert((expression))
#endif
*/

void assert(int);
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

void assert(int expression) {
    if (!expression) {
        DebugBreak();
    }
}