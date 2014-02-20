// ServiceServerObjectImpl.cpp : Implementation of CServiceServerObjectImpl

#include "stdafx.h"
#include "ServiceServerObjectImpl.h"


// CServiceServerObjectImpl

HRESULT CServiceServerObjectImpl::FinalConstruct()
{
	boost::log::add_file_log
			(
			keywords::file_name = "log_%N.log",
			keywords::rotation_size = LOG_ROT_SIZE,
			keywords::time_based_rotation =
			logging::sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::format = "[%TimeStamp%]: %Message%"
			);

		logging::add_common_attributes();

 	return S_OK;
}

void CServiceServerObjectImpl::ThreadedServiceOperations(std::wstring servName, int serviceComand)
{
 	 		
 	boost::lock_guard<boost::mutex> lock(mRecurciveMutex);
 
 	src::logger_mt& lg = my_logger::get();
 	WinServiceManager& sm = WinServiceManager::Instance();

	sm.openSCManager();
 
 	BOOST_LOG(lg) << " Start managing service: '"<<servName << "' in new thread" 
 		<<  boost::this_thread::get_id();
	
	BSTR bSername = SysAllocStringLen(servName.data(), servName.size());
 	try
 	{

		if (START_SERVICE == serviceComand) {
 		    sm.startMyService(bSername);
			std::wstring sucRes = servName + (L" service successfully started");
 			Fire_ReportError(SysAllocStringLen(sucRes.data(), sucRes.size()));
 			BOOST_LOG(lg) << sucRes  ;
 
 		}
 		else {
 			sm.stopMyService(bSername);
			std::wstring sucRes = servName + (L" service successfully stopped");
			Fire_ReportError(SysAllocStringLen(sucRes.data(), sucRes.size()));
 			BOOST_LOG(lg) << sucRes  ;
 		}
 	
 		
 	}
 	catch (BSTR serviceError)
 	{
 		assert(serviceError != NULL);
 
 		CComBSTR bstrDesc(serviceError);
 		USES_CONVERSION;
 		std::string strDesc = static_cast<CHAR*>(CW2A(bstrDesc));

 		BOOST_LOG(lg) << "An error has occured: "<< strDesc << servName;	
		CComBSTR fullError  = L"An error has occured: ";
		fullError.Append(serviceError);
		fullError.Append(bSername);
 		Fire_ReportError(fullError);
 	}
}


STDMETHODIMP CServiceServerObjectImpl::startService(BSTR serviceName)
{
	assert(serviceName != NULL);
	std::wstring wServiceName(serviceName, SysStringLen(serviceName));	
	boost::thread ServiceThread(&CServiceServerObjectImpl::ThreadedServiceOperations,
		this, wServiceName,START_SERVICE);
	return S_OK;
}


STDMETHODIMP CServiceServerObjectImpl::stopService(BSTR serviceName)
{
	assert(serviceName != NULL);
	std::wstring wServiceName(serviceName, SysStringLen(serviceName));	
	boost::thread ServiceThread(&CServiceServerObjectImpl::
		ThreadedServiceOperations,
		this, wServiceName,STOP_SERVICE);

	return S_OK;
}
