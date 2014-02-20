 #ifndef _EventFiringObject_WinServiceManager
 #define _EventFiringObject_WinServiceManager
 
#include <string>

 /**
  * @brief Thread-safe (C11 standart)
  * Service manager class
  */
 
 class WinServiceManager
 {
 public:
 	static WinServiceManager& Instance()
 	{
 		
 		static WinServiceManager theSingleWinServiseManager;
 		return theSingleWinServiseManager;
 	}
 
 	/**
 	 * @brief Open service handler
	 * full access to services
 	 **/
 
 	void openSCManager();

	 /**
 	 * @brief start service by its name with panding
	 * @paramIn service name, provided by user
	 * 
 	 **/

	void startMyService(BSTR service);
	/**
 	 * @brief stop service by its name with panding
	 * stop dependencies if needs
	 * @paramIn service name, provided by user
	 * 
 	 **/

	void stopMyService(BSTR servName);

 private: 

	 //pending time config

	static const DWORD TIME_OUT_STOP_PENDING = 4000;
	static const DWORD TIME_OUT_FT = 1000;
	static const DWORD TIME_OUT_ST = 10000;
	static const DWORD CHECK_TIME_PENDING = 10;
 	SC_HANDLE schSCManager;

 	~WinServiceManager();
	WinServiceManager(){};
 	WinServiceManager(WinServiceManager const&);
 	WinServiceManager& operator=( WinServiceManager const&);
 
	/**
 	 * @brief Service handler wrapper
	 * 
 	 **/	

	class ScopedService {
	
	private:

		SC_HANDLE schService;
		ScopedService(ScopedService const& p);
		ScopedService& operator=(ScopedService const& p);

	public:
		ScopedService(SC_HANDLE handle, LPCWSTR servName, DWORD accessParams) 
		{
			
		    schService = OpenService(handle, servName,       
		    accessParams);
			if(schService == NULL)
			{
			    throw(L"Can't open service ");
			}
		}

		bool isNull() 
		{
			return schService == NULL;
		}
		SC_HANDLE& getService() 
		{
			return schService;
		}

		~ScopedService()
		{
			if (CloseServiceHandle(schService) == 0)
			{
				throw(L"CloseServiceHandle() error ");
			}
		}
	};

	/**
 	 * @brief Dependensies wrapper
	 * 
 	 **/

	class ScopedProcessHeap {
	
	private:

		LPENUM_SERVICE_STATUS sSScopedHeap;
		ScopedProcessHeap(ScopedProcessHeap const& p);
		ScopedProcessHeap& operator=(ScopedProcessHeap const& p);

	public:
		ScopedProcessHeap(HANDLE handle, DWORD flags, size_t bytes) 
		{		
		    sSScopedHeap = (LPENUM_SERVICE_STATUS) 
			HeapAlloc(handle, 
			flags, bytes);

			if(sSScopedHeap == NULL) 
			{
			    throw(L"Can't construct process heap ");
			}
		}

		bool isNull() 
		{
			return sSScopedHeap == NULL;
		}

		LPENUM_SERVICE_STATUS& getService() 
		{
			return sSScopedHeap;
		}

		~ScopedProcessHeap()
		{
			if (HeapFree(GetProcessHeap(), 0, sSScopedHeap) == 0)
			{
				throw(L"HeapFree error ");
			}
		}
	};
 
 
 };
 
 #endif //_EventFiringObject_WinServiceManager