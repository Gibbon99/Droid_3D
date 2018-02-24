#pragma once

// Clean up and shutdown everything
void sys_shutdownToSystem();

// Display a message box indicating non-fatal error
void sysErrorNormal(const char *sourcefile, int sourceline, const char *text, ...);

// Display a message box indicating a fatal error
void sysErrorFatal(const char *sourcefile, int sourceline, const char *text, ...);
