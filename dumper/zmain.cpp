// zclient.cpp : Defines the entry point for the application.
// duccom0123 created this file.

#include <string>
#include <thread>
#include "zmain.h"
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include "import.h" 
#include "zUtil.h" 
#include <direct.h>




using namespace std;
using namespace std::chrono;
FILE* f;


extern "C" _declspec(dllexport) void InitDumper() // OK
{
#if DEBUGCONSOLE
	AllocConsole();
	SetConsoleOutputCP(1252);
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif
	

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		FreeLibrary(hModule);
	}
	break;
	}
	return TRUE;
}
