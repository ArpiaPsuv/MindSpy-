#pragma once
#ifndef WINREG_H
#define WINREG_H

namespace MindSpy
{
	typedef struct RegData
	{
		HKEY rootKey;
		LPWSTR subKey;
		LPWSTR value;
	}RegData;


	class WinReg
	{
	public:
		WinReg();
		~WinReg();

		DWORD _regReadValue(RegData* regData, LPDWORD typeBuff, PVOID Buffer, DWORD sz);

	private:
		DWORD errcode;
	};


}
#endif // WINREG_H
