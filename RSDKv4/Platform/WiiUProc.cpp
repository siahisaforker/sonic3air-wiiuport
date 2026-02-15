#include "WiiUProc.hpp"

#if RETRO_PLATFORM == RETRO_WIIU

#if defined(RETRO_WIIU_CF_AROMA)
// Aroma CFW: provide compatible stubs (adjust if Aroma exposes a different API)
void WiiU_ProcInit() { }
void WiiU_ProcShutdown() { }
bool WiiU_ProcIsRunning() { return true; }


#else
// Prefer WUT/ProcUI when available; otherwise fall back to WHB helpers.
#if defined(__WUT__)
#include <proc_ui/procui.h>
#include <coreinit/foreground.h>

static void WiiU_ProcSaveCallback(void) { OSSavesDone_ReadyToRelease(); }

void WiiU_ProcInit() { ProcUIInit(WiiU_ProcSaveCallback); }
void WiiU_ProcShutdown() { ProcUIShutdown(); }
bool WiiU_ProcIsRunning() { return ProcUIIsRunning() && !ProcUIInShutdown(); }

#else
// WHB fallback (older toolchains)
#include <whb/proc.h>

void WiiU_ProcInit() { WHBProcInit(); }
// For graceful exit under WHB/Tiramisu, signal the proc loop to stop
// rather than forcibly shutting down low-level subsystems.
void WiiU_ProcShutdown() { WHBProcStopRunning(); }
bool WiiU_ProcIsRunning() { return WHBProcIsRunning(); }

#endif

// Default weak hooks for foreground release/acquire. Linkers on platforms
// that need to free MEM1 or other foreground-only resources can override
// these symbols; they are no-ops by default.
#if RETRO_PLATFORM == RETRO_WIIU
extern "C" void WiiU_OnReleaseForeground() __attribute__((weak));
extern "C" void WiiU_OnAcquireForeground() __attribute__((weak));

void WiiU_OnReleaseForeground() { }
void WiiU_OnAcquireForeground() { }
#endif

#endif // RETRO_WIIU_CF_AROMA

#else
// Non-WiiU platforms: no-op
void WiiU_ProcInit() { }
void WiiU_ProcShutdown() { }
bool WiiU_ProcIsRunning() { return true; }
#endif
