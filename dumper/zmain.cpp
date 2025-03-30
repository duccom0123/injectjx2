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
		push 0xFFEF5F5F // mã màu
		lea ecx, [edi + 0x198] // vị trí hiển thị
		call callFunc
		push 0xFF4497E8 // mã màu
		lea ecx, [edi + (0x198 + 0x680)] // vị trí hiển thị
		call callFunc
		push 0xFF1BFA48 // mã màu
		lea ecx, [edi + (0x198 + 0x680*2)] // vị trí hiển thị
		call callFunc

		jmp jmpBack
	}
}

__declspec(naked) void HookedTextAttrs()
{
	static DWORD jmpBack	= 0x0074BD9E;
	static DWORD sub_6A0000 = 0x006A0000;
	__asm
	{
		///hp
		mov ecx, [edi + 0xC4] //hp min
		mov edx, [edi + 0xC8] //hp max
		push 0x2F
		push ecx
		push edx
		lea ecx, [esi + 0x198] // vị trí hiển thị
		call sub_6A0000
		///mp
		mov eax, [edi + 0xCC] //mp min
		mov ecx, [edi + 0xD0] //mp max
		push 0x2F
		push eax
		push ecx
		lea ecx, [esi + (0x198 + 0x680)] // vị trí hiển thị
		call sub_6A0000
		///stamina
		mov ecx, [edi + 0xD4] //stamina min
		mov edx, [edi + 0xD8] //stamina max
		push 0x2F
		push ecx
		push edx
		lea ecx, [esi + (0x198 + 0x680*2)] // vị trí hiển thị
		call sub_6A0000
		///Pdame
		mov ecx, [edi + 0x4C] //Pdame min
		mov edx, [edi + 0x48] //Pdame max
		push 0x2D
		push ecx
		push edx
		lea ecx, [esi + (0x198 + 0x680 * 3)] // vị trí hiển thị
		call sub_6A0000

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
	SetOp((LPVOID)0x0074BB70, (LPVOID)HookedTextAttrs, ASM::JMP);

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
