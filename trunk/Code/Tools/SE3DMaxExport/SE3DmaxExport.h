#ifndef _SE3DmaxExport_H_
#define _SE3DmaxExport_H_

#include <vector>
#include <string>
#include "max.h"

class SE3DmaxExport : public SceneExport
{
public:
	SE3DmaxExport();
	~SE3DmaxExport();

	// SceneExport methods
	int ExtCount();
	TCHAR const * Ext(int n);     
	TCHAR const * LongDesc();
	TCHAR const * ShortDesc();
	TCHAR const * AuthorName(); 
	TCHAR const * CopyrightMessage(); 
	TCHAR const * OtherMessage1(); 
	TCHAR const * OtherMessage2(); 
	unsigned int Version();
	void ShowAbout(HWND wnd);
	int DoExport(TCHAR const * name, ExpInterface* exp_interface, Interface* max_interface, BOOL suppress_prompts, DWORD options);
	BOOL SupportsOptions(int ext, DWORD options);

private:
	static INT_PTR WINAPI ExportWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void EnumNode(INode* node);

private:
	std::string m_sFileName;
	Interface* m_pInterface;
	std::vector<INode*> m_ExportNodes;
};

class SE3DmaxExportClassDesc : public ClassDesc
{
public:
	int IsPublic()
	{
		return TRUE;
	}

	void* Create(BOOL loading)	
	{
		return new SE3DmaxExport;
	}

	TCHAR const * ClassName()
	{
		return _T("SE3D Export");
	}
	SClass_ID SuperClassID()
	{
		return SCENE_EXPORT_CLASS_ID;
	}
	Class_ID ClassID()
	{
		return Class_ID(0x509c6516, 0x128d15a7);
	}
	TCHAR const * Category()
	{
		return _T("");
	}
};


#endif