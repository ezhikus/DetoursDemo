#include <windows.h>
#include "C:\Program Files\Microsoft Research\Detours Express 3.0\src\detours.h"

LRESULT (WINAPI * TrueSendMessageW)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = SendMessageW;

__declspec(dllexport) LRESULT WINAPI MySendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_SETTEXT && wcsstr((LPCTSTR)lParam, L"/ Хабрахабр - Mozilla Firefox") != NULL)
		return TrueSendMessageW(hWnd, Msg, wParam, (LPARAM)L"Привет, Хабр!");

	return TrueSendMessageW(hWnd, Msg, wParam, lParam);
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) 
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueSendMessageW, MySendMessageW);
		DetourTransactionCommit();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueSendMessageW, MySendMessageW);
		DetourTransactionCommit();
	}
	return TRUE;
}