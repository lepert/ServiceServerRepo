// ServiceServerObjectImpl.h : Declaration of the CServiceServerObjectImpl

#ifndef _ServiceServer_ServiceServerObjectImpl_H
#define _ServiceServer_ServiceServerObjectImpl_H

//main include files

#include "resource.h"       // main symbols
#include "ServiceServer_i.h"
#include "_IServiceServerObjectImplEvents_CP.h"
#include "WinServiceManager.h"

//threads

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/lockable_concepts.hpp>

//log

#include <boost/move/utility.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

//strings

#include <iostream>
#include <assert.h>
#include <string>
#include <comutil.h>


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::logger_mt)

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CServiceServerObjectImpl

class ATL_NO_VTABLE CServiceServerObjectImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CServiceServerObjectImpl, &CLSID_ServiceServerObjectImpl>,
	public IConnectionPointContainerImpl<CServiceServerObjectImpl>,
	public CProxy_IServiceServerObjectImplEvents<CServiceServerObjectImpl>,
	public IDispatchImpl<IServiceServerObjectImpl, &IID_IServiceServerObjectImpl, &LIBID_ServiceServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CServiceServerObjectImpl()
	{	}

DECLARE_REGISTRY_RESOURCEID(IDR_SERVICESERVEROBJECTIMPL)

DECLARE_NOT_AGGREGATABLE(CServiceServerObjectImpl)

BEGIN_COM_MAP(CServiceServerObjectImpl)
	COM_INTERFACE_ENTRY(IServiceServerObjectImpl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CServiceServerObjectImpl)
	CONNECTION_POINT_ENTRY(__uuidof(_IServiceServerObjectImplEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	/**
	 *  @brief presettings of
	 * COM sets log
	 **/

	HRESULT FinalConstruct();

	void FinalRelease()
	{
	}

public:


	enum
	{
	    START_SERVICE = 0, 
		STOP_SERVICE
	}ServiceCommands;

	boost::mutex mRecurciveMutex;

	/**
	 * @brief thread guarded with boost lock
	 * starts/stops asynchronously services 
	 * using WinServiceManager instance
	 * throws exeptions as an server event 
	 * writes events to log (see TargetDir log_0)
	 * @paramIn: 
	 * - wstring service Name 
	 * - int Service Commands ( see enum ServiceCommands) 
	 * TODO: incapsulate call by command pattern
	 **/

	void ThreadedServiceOperations(std::wstring ,int); 

	
	/**
	 * @brief creates thread to start service
	 * asynchronous processing
	 * @paramIn: 
	 * - BSTR serviceName 
	 **/

	STDMETHOD(startService)(BSTR serviceName);

	/**
	 * @brief creates thread to stop service
	 * asynchronous processing
	 * @paramIn: 
	 * - BSTR serviceName 
	 **/

	STDMETHOD(stopService)(BSTR serviceName);
private :
	static const size_t LOG_ROT_SIZE = 10 * 1024 * 1024;
};

OBJECT_ENTRY_AUTO(__uuidof(ServiceServerObjectImpl), CServiceServerObjectImpl)

#endif