// dllmain.h : Declaration of module class.

class CServiceServerModule : public ATL::CAtlDllModuleT< CServiceServerModule >
{
public :
	DECLARE_LIBID(LIBID_ServiceServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SERVICESERVER, "{AC5E46D4-1CB4-4DC7-BEF6-E965925A1EC3}")
};

extern class CServiceServerModule _AtlModule;
