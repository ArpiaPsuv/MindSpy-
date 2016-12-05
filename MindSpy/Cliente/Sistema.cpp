#include "Sistema.h"

namespace MindSpy
{
	char* Sistema::ObtenerMAC()
	{
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		static char mac_addr[18];
		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
			free(AdapterInfo);
			AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		}

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
			do {
				if (strcmp(AdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0"))
				{
					sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
						AdapterInfo->Address[0], AdapterInfo->Address[1],
						AdapterInfo->Address[2], AdapterInfo->Address[3],
						AdapterInfo->Address[4], AdapterInfo->Address[5]);

					return mac_addr;
				}
				AdapterInfo = AdapterInfo->Next;
			} while (AdapterInfo);
		}
		free(AdapterInfo);
	}

	UINT32 Sistema::ObtenerVersionWindows()
	{
		
	}
}