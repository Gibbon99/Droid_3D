#pragma once

extern bool 		scriptEngineStarted;
extern unsigned int	numFunctionsInScripts;
extern unsigned int numHostScriptFunctions;

bool util_startScriptEngine();
bool sys_registerVariables();
bool util_registerFunctions();
bool util_loadAndCompileScripts();
bool util_cacheFunctionIDs();
