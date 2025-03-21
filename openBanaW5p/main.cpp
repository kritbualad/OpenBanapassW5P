#include "includes.h"

static uintptr_t imageBase;

inline void safeJMP(injector::memory_pointer_tr at, injector::memory_pointer_raw dest, bool vp = true)
{
	MH_Initialize();
	MH_CreateHook((void*)at.as_int(), (void*)dest.as_int(), nullptr);
	MH_EnableHook((void*)at.as_int());
}

inline void safeUNJMP(injector::memory_pointer_tr pTarget) {
	MH_DisableHook((void*)pTarget.as_int());
	MH_RemoveHook((void*)pTarget.as_int());
}

static int returnTrue() {
	return 1;
}

static int returnFalse() {
	return 0;
}


bool init = false;

/*
DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		
	} while (!init_hook);
	return TRUE;
}
*/


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		imageBase = (uintptr_t)GetModuleHandleA(0);
		//behold... my shitty hooks!!
		safeJMP(imageBase + 0xCA88C0, BngRwAttach);
		safeJMP(imageBase + 0xCA7F80, BngRwInit);
		safeJMP(imageBase + 0xCA8010, BngRwFin);
		safeJMP(imageBase + 0xCA88B0, BngRwGetVersion);
		safeJMP(imageBase + 0xCA80E0, BngRwIsCmdExec);
		safeJMP(imageBase + 0xCA85E0, BngRwReqLed);
		safeJMP(imageBase + 0xCA87D0, BngRwReqAction);
		safeJMP(imageBase + 0xCA86D0, BngRwReqBeep);
		safeJMP(imageBase + 0xCA8150, BngRwReqCancel);
		safeJMP(imageBase + 0xCA8330, BngRwReqSendUrlTo);
		safeJMP(imageBase + 0xCA8220, BngRwReqWaitTouch);

		//CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;



	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}