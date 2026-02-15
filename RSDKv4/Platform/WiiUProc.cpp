#include "WiiUProc.hpp"

#if RETRO_PLATFORM == RETRO_WIIU

#if defined(RETRO_WIIU_CF_AROMA)
// Aroma CFW: provide compatible stubs (adjust if Aroma exposes a different API)
void WiiU_ProcInit() { }
void WiiU_ProcShutdown() { }
bool WiiU_ProcIsRunning() { return true; }

#else
// Default: use WHB proc functions (Tiramisu/standard wiiu homebrew)
#include <whb/proc.h>

void WiiU_ProcInit() { WHBProcInit(); }
// For graceful exit under WHB/Tiramisu, signal the proc loop to stop
// rather than forcibly shutting down low-level subsystems.
void WiiU_ProcShutdown() { WHBProcStopRunning(); }
bool WiiU_ProcIsRunning() { return WHBProcIsRunning(); }

#endif // RETRO_WIIU_CF_AROMA

#else
// Non-WiiU platforms: no-op
void WiiU_ProcInit() { }
void WiiU_ProcShutdown() { }
bool WiiU_ProcIsRunning() { return true; }
#endif
