 #include "stdafx.h"
 #include "WinServiceManager.h"
  
 void WinServiceManager::openSCManager()
 {
	 schSCManager = OpenSCManager(
		 NULL,                    
		 NULL,                    
		 SC_MANAGER_ALL_ACCESS);  

	if (schSCManager == NULL)
	{
			 throw(L"OpenSCManager() failed ");
	}
 }

 WinServiceManager::~WinServiceManager()
 {
	 CloseServiceHandle(schSCManager);
 }



 void WinServiceManager::startMyService(BSTR bServName)
 {
	SERVICE_STATUS ssStatus;
	DWORD dwOldCheckPoint;
	DWORD dwStartTickCount;
	DWORD dwWaitTime;
	
	USES_CONVERSION; 
	LPCWSTR pString = OLE2CW(bServName); 

	ScopedService schService(schSCManager, pString, SERVICE_ALL_ACCESS);

	  if (!StartService(schService.getService(),  0, NULL)) 
	  {
		 throw (L"StartService() failed, error ");
	  }

	 if (!QueryServiceStatus(schService.getService(), &ssStatus)) 
	 {
	     throw(L"StartService(), service still start pending ");
     }
	 else
	 {
	     dwStartTickCount = GetTickCount();
	     dwOldCheckPoint = ssStatus.dwCheckPoint;
	     while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
		 {
		     dwWaitTime = ssStatus.dwWaitHint / CHECK_TIME_PENDING;
		     if (dwWaitTime < TIME_OUT_FT)
			 {
			     dwWaitTime = TIME_OUT_FT;
			 }
		     else if (dwWaitTime > TIME_OUT_ST)
			 {
			     dwWaitTime = TIME_OUT_ST;
			 }
		     Sleep(dwWaitTime);

		     if (!QueryServiceStatus(schService.getService(), &ssStatus))  
			 {
			     break;
			 }
		     if (ssStatus.dwCheckPoint > dwOldCheckPoint) 
			 {
			     dwStartTickCount = GetTickCount();
			     dwOldCheckPoint = ssStatus.dwCheckPoint;
		     }
		     else
			 {
			     if ((GetTickCount() - dwStartTickCount) > ssStatus.dwWaitHint) 
				 {
				     break;
			     }
		     }
	     }
	 }

	 if (ssStatus.dwCurrentState != SERVICE_RUNNING)
	     throw (L"Service not started ");

 }

 void WinServiceManager::stopMyService(BSTR bServName) {
	
	SERVICE_STATUS ssStatus;
	DWORD dwStartTime = GetTickCount();
	DWORD i;
	DWORD dwBytesNeeded;
	DWORD dwCount;
	ENUM_SERVICE_STATUS     ess;

	USES_CONVERSION; 
	LPCWSTR pString = OLE2CW(bServName); 

	ScopedService schService(schSCManager, pString, SERVICE_ALL_ACCESS);

	if (QueryServiceStatus(schService.getService(), &ssStatus))
	{
	    if (ssStatus.dwCurrentState == SERVICE_STOPPED)
		{
			throw(L"Service already stopped ");
		}
	}

	while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
	{
		Sleep(ssStatus.dwWaitHint);
		if (QueryServiceStatus(schService.getService(), &ssStatus)) 
		{
			if (ssStatus.dwCurrentState == SERVICE_STOPPED)
			{
				throw(L" Service panding error ");
			}
		    if (GetTickCount() - dwStartTime > TIME_OUT_STOP_PENDING)
			{
			    throw(L"Timeout error ");
			}
		}
	}

	 if ((EnumDependentServices(schService.getService(), 
		SERVICE_ACTIVE, NULL, 
		0, &dwBytesNeeded, &dwCount)) == 0) 
	 {

		if (GetLastError() != ERROR_MORE_DATA)
		{
			throw(L"Unexpected  error");
		}
		
		ScopedProcessHeap lpDependencies(GetProcessHeap(), 
			HEAP_ZERO_MEMORY, dwBytesNeeded);
		
		if (lpDependencies.isNull())
		{
			throw(L"Error allocating buffer for the dependencies ");
		}
		
		if (!EnumDependentServices(schService.getService(),
			SERVICE_ACTIVE, lpDependencies.getService(),
			dwBytesNeeded, &dwBytesNeeded, 
			&dwCount))
		{
				throw(L"Unable to  enumerate dependencies ");
		}
			
		for (i = 0; i < dwCount; i++)
		{
			ess = *(lpDependencies.getService() + i);

			ScopedService hDepService(schSCManager, ess.lpServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);

			if (!ControlService(hDepService.getService(), SERVICE_CONTROL_STOP, &ssStatus))
			{
					throw(L"Unable to  enumerate dependencies ");
			}
							
			while (ssStatus.dwCurrentState != SERVICE_STOPPED) 
			{
				Sleep(TIME_OUT_ST);
				if (!QueryServiceStatus(hDepService.getService(), &ssStatus))
				{
					if (ssStatus.dwCurrentState == SERVICE_STOPPED)
					{
							// Dependency was stopped
						break;
					}
					if (GetTickCount() - dwStartTime > TIME_OUT_STOP_PENDING)
					{
							throw(L"Timout dependency error ");
					}
				}
			}			
		}
	}
 
	 if (!ControlService(schService.getService(), SERVICE_CONTROL_STOP, &ssStatus))
	 {
		throw(L"Dependency closed incorrectly ");
	 }

	while (ssStatus.dwCurrentState != SERVICE_STOPPED) {
		Sleep(TIME_OUT_ST);
		if (!QueryServiceStatus(schService.getService(), &ssStatus)) {
			if (ssStatus.dwCurrentState == SERVICE_STOPPED)
			{
				break;
			}
			if (GetTickCount() - dwStartTime > TIME_OUT_STOP_PENDING)
			{
				throw(L"Error Timeout ");
			}
		}
	}
 }





