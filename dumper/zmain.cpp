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

__declspec(naked) void HookedStamina()
{
	static DWORD jmpBack	= 0x0074B2EC;
	static DWORD callFunc	= 0x0072EDC0;
	__asm
	{
		push 0xFFEF5F5F
		lea ecx, [edi + 0x198]
		call callFunc
		push 0xFF4497E8
		lea ecx, [edi + (0x198 + 0x680)]
		call callFunc
		push 0xFF1BFA48
		lea ecx, [edi + (0x198 + 0x680*2)]
		call callFunc

		jmp jmpBack
	}
}

__declspec(naked) void HookedTextAttrs()
{
	static DWORD jmpBack	= 0x0074BE56;
	__asm
	{
		jmp jmpBack
	}
}

extern "C" _declspec(dllexport) void InitDumper() // OK
{
#if DEBUGCONSOLE
	AllocConsole();
	SetConsoleOutputCP(1252);
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif
	SetOp((LPVOID)0x0074B2CC, (LPVOID)HookedStamina, ASM::JMP);
	SetOp((LPVOID)0x0074BAD8, (LPVOID)HookedTextAttrs, ASM::JMP);

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
