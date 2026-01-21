#include <windows.h>

// Per-process instance handle.
HINSTANCE g_hInstance = nullptr;

// Called on every process attach/detach.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hModule;
        // Disable calling on every thread attach/detach (costly, needless).
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
