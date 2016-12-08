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

		/**
		 * \brief 
		 * \param regData 
		 * \param typeBuff 
		 * \param Buffer 
		 * \param sz 
		 * \return 
		 */

		DWORD regReadValue(RegData* regData, LPDWORD typeBuff, PVOID Buffer, DWORD sz);

		bool regSetValue(RegData* regData, DWORD type, const void* buffer, DWORD bufferSize);
		/**
		 * \brief 
		 * \param data 
		 * \param DataBuff 
		 * \return 
		 */
		DWORD getRegDwordValue(RegData* data, void* DataBuff);
		
		/**
		 * \brief 
		 * \param data 
		 * \param buffer 
		 * \param bufferSz 
		 * \return 
		 */
		DWORD getRegStrinData(RegData* data, LPWSTR buffer, DWORD bufferSz);

		bool setRegValueDword(RegData* rgdata, DWORD data);


	private:
		DWORD errcode;
		DWORD type;
	};


}
#endif // WINREG_H
