#include <Windows.h>
#include <ntstatus.h>
#include <bcrypt.h>
#pragma comment (lib, "Bcrypt.lib")

#include "../basic/basic.c"

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

void crypto_random(u8* out, u32 length) {
    NTSTATUS status = BCryptGenRandom(
        NULL,
        out,
        length,
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );
    assert(status == STATUS_SUCCESS);
}

void trigger_breakpoint() {
    DebugBreak();
}