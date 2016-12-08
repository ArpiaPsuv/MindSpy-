#include "Windows.h"
#include "winreg.h"


namespace MindSpy
{

	WinReg::WinReg()
	{


	}

	WinReg::~WinReg()
	{


	}
	

	/**
	 * \brief 
	 * \param regData 
	 * \param typeBuff 
	 * \param Buffer 
	 * \param sz 
	 * \return 
	 */
	DWORD WinReg::_regReadValue(RegData* regData, LPDWORD typeBuff, PVOID Buffer, DWORD sz)
	{

		errcode = RegOpenKeyExW(regData->rootKey, regData->subKey, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &regData->rootKey);
		if (errcode == ERROR_SUCCESS)
		{
			errcode = RegQueryValueExW(regData->rootKey, regData->value, 0, typeBuff, LPBYTE(Buffer), &sz);
			if (errcode != ERROR_SUCCESS)return errcode;
			
		}
		return errcode;
	}
}
