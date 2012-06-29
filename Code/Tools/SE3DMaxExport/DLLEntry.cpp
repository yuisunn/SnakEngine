#include "SE3DmaxExport.h"

SE3DmaxExportClassDesc mcd;

HINSTANCE dll_instance;

BOOL APIENTRY DllMain(HINSTANCE module, DWORD ul_reason_for_call, LPVOID reserved)
{
	dll_instance = module;
	return TRUE;
}

extern "C" int LibNumberClasses()
{
	return 1;
}

extern "C" ClassDesc* LibClassDesc(int i)
{
	switch (i)
	{
	case 0:
		return &mcd;

	default:
		return 0;
	}
}

extern "C" const TCHAR* LibDescription()
{
	return TEXT("SEModel XML File Export Plugin");
}

extern "C" ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

extern "C" ULONG CanAutoDefer()
{
   return 1;
}