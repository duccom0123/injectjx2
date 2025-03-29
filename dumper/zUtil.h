// duccom0123 created this file.
#pragma once
#include <windows.h>
namespace ASM
{
	enum T
	{
		JMP = 0xE9,
		JE = 0x74,
		JNE = 0x75,
		JGE = 0x7D,
		NOP = 0x90,
		CALL = 0xE8,
		CALLD = 0xFF,
		PUSH = 0x68,
	};
};

void SetByte(DWORD offset,BYTE value);
void SetWord(DWORD offset,WORD value);
void SetDword(DWORD offset,DWORD value);
void SetInt(DWORD offset,DWORD value);
void SetCompleteHook(BYTE head,DWORD offset,...);
DWORD SetOp(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress, const BYTE cmd);
DWORD SetRange(const LPVOID dwAddress, const USHORT wCount, const BYTE btValue);
void MemoryCpy(DWORD offset,void* value,DWORD size);
void MemorySet(DWORD offset,DWORD value,DWORD size);
void VirtualizeOffset(DWORD offset,DWORD size);
void* DetourFunc(BYTE* src, const BYTE* dst, const int len);
void __stdcall HookFunction(PVOID* oFunction, PVOID pDetour);