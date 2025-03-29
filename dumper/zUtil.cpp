// duccom0123 created this file.
#include "zUtil.h"
#include "detours.h" 
#include <atomic>

DWORD WriteMemory(const LPVOID lpAddress, const LPVOID lpBuf, const UINT uSize)
{
	DWORD dwErrorCode = 0;
	DWORD dwOldProtect = 0;
	// ----
	int iRes = VirtualProtect(lpAddress, uSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// ----
	if (iRes == 0)
	{
		dwErrorCode = GetLastError();
		return dwErrorCode;
	}
	// ----
	memcpy(lpAddress, lpBuf, uSize);
	// ----
	DWORD dwBytes = 0;
	// ----
	iRes = VirtualProtect(lpAddress, uSize, dwOldProtect, &dwBytes);
	// ----
	if (iRes == 0)
	{
		dwErrorCode = GetLastError();
		return dwErrorCode;
	}
	// ----
	return 0x00;
}
void SetByte(DWORD offset,BYTE value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,1,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(BYTE*)(offset) = value;

	VirtualProtect((void*)offset,1,OldProtect,&OldProtect);
}
void SetWord(DWORD offset,WORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,2,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(WORD*)(offset) = value;

	VirtualProtect((void*)offset,2,OldProtect,&OldProtect);
}
void SetDword(DWORD offset,DWORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(DWORD*)(offset) = value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}
void SetInt(DWORD offset, DWORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 4, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(int*)(offset) = value;

	VirtualProtect((void*)offset, 4, OldProtect, &OldProtect);
}
void SetCompleteHook(BYTE head,DWORD offset,...) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,5,PAGE_EXECUTE_READWRITE,&OldProtect);

	if(head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset+1;

	*(DWORD*)(offset+1) = (*function)-(offset+5);

	VirtualProtect((void*)offset,5,OldProtect,&OldProtect);
}
DWORD SetOp(const LPVOID dwEnterFunction, const LPVOID dwJMPAddress, const BYTE cmd)
{
	BYTE btBuf[5];
	DWORD dwShift = (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 5;
	// ----
	btBuf[0] = cmd;
	memcpy((LPVOID)&btBuf[1], (LPVOID)&dwShift, sizeof(ULONG_PTR));
	// ----
	return WriteMemory(dwEnterFunction, (LPVOID)btBuf, sizeof(btBuf));
}
DWORD SetRange(const LPVOID dwAddress, const USHORT wCount, const BYTE btValue)
{
	BYTE* lpBuf = new BYTE[wCount];
	// ----
	memset(lpBuf, btValue, wCount);
	// ----
	return WriteMemory(dwAddress, (LPVOID)lpBuf, wCount);
}
void MemoryCpy(DWORD offset,void* value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memcpy((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}
void MemorySet(DWORD offset,DWORD value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memset((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}
void VirtualizeOffset(DWORD offset,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	DWORD HookAddr = (DWORD)malloc(size+5);

	memcpy((void*)HookAddr,(void*)offset,size);

	*(BYTE*)(HookAddr+size) = 0xE9;

	*(DWORD*)(HookAddr+size+1) = (offset+size)-((HookAddr+size)+5);

	*(BYTE*)(offset) = 0xE9;

	*(DWORD*)(offset+1) = HookAddr-(offset+5);

	memset((void*)(offset+5),0x90,(size-5));

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}
void* DetourFunc(BYTE* src, const BYTE* dst, const int len)
{
	BYTE* jmp = (BYTE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len + 5);

	if (src[0] != 0xE9)
	{
		DWORD dwBack;

		VirtualProtect(src, len, PAGE_READWRITE, &dwBack);

		memcpy(jmp, src, len);

		jmp += len;
		jmp[0] = 0xE9;
		*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
		src[0] = 0xE9;
		*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;

		VirtualProtect(src, len, dwBack, &dwBack);

		return (jmp - len);
	}

	return 0;
}
void __stdcall HookFunction(PVOID* oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}
