//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
//				3DXI file exporter project template
//				For a more in-depth exemple of a 3DXI exporter,
//				please refer to maxsdk\samples\igame\export.
// AUTHOR:		Jean-Francois Yelle - created Mar.20.2007
//***************************************************************************/


#include "SE3DmaxExport.h"
#include "XMLExport.h"
#include "XMLUtility.h"
#include <WinUser.h>
#include "resource.h"

#include <max.h>
#include <Interval.h>

#include <vector>
#include <sstream>

extern HINSTANCE dll_instance;

SE3DmaxExport::SE3DmaxExport()
{
}

SE3DmaxExport::~SE3DmaxExport()
{
}

int SE3DmaxExport::ExtCount()
{
	return 1;
}

TCHAR const * SE3DmaxExport::Ext(int n)
{
	switch (n)
	{
	case 0:
		return TEXT("SEM");

	default:
		return NULL;
	}
}

TCHAR const * SE3DmaxExport::LongDesc()
{
	return TEXT("SEModel XML File");
}

TCHAR const * SE3DmaxExport::ShortDesc()
{
	return TEXT("SEModel File");
}

TCHAR const * SE3DmaxExport::AuthorName()
{
	return TEXT("slc");
}

TCHAR const * SE3DmaxExport::CopyrightMessage()
{
	return TEXT("Copyright 20010-2011");
}

TCHAR const * SE3DmaxExport::OtherMessage1()
{
	return TEXT("");
}

TCHAR const * SE3DmaxExport::OtherMessage2()
{
	return TEXT("");
}

unsigned int SE3DmaxExport::Version()
{
	return 010;
}

void SE3DmaxExport::ShowAbout(HWND hWnd)
{
	::MessageBox(hWnd, TEXT("slc, Copyright 2009-2011"), TEXT("About"), MB_OK);
}

BOOL SE3DmaxExport::SupportsOptions(int ext, DWORD options)
{
	if ((options & SCENE_EXPORT_SELECTED) && (0 == ext))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int SE3DmaxExport::DoExport(TCHAR const * name, ExpInterface* exp_interface, Interface* max_interface,
						BOOL suppress_prompts, DWORD options)
{
	m_sFileName = tstr_to_str(name);

	m_pInterface = max_interface;

	m_ExportNodes.clear();
	if (options & SCENE_EXPORT_SELECTED)
	{
		int count = m_pInterface->GetSelNodeCount();
		for (int i = 0; i < count; ++ i)
		{
			this->EnumNode(m_pInterface->GetSelNode(i));
		}
	}
	else
	{
		this->EnumNode(this->m_pInterface->GetRootNode());
	}

	HWND max_wnd = max_interface->GetMAXHWnd();
	if (::DialogBoxParam(dll_instance, MAKEINTRESOURCE(IDD_MESHML_EXPORT), max_wnd,
		ExportWndProc, reinterpret_cast<LPARAM>(this)))
	{
		MessageBox(max_wnd, TEXT("导出成功！"), TEXT("SEModel Export"), MB_OK);
	}

	return 1;
}

void SE3DmaxExport::EnumNode(INode* node)
{
	if (!node->IsNodeHidden())
	{
		m_ExportNodes.push_back(node);
	}

	int num_children = node->NumberOfChildren();
	for (int i = 0; i < num_children; ++ i)
	{
		this->EnumNode(node->GetChildNode(i));
	}
}

INT_PTR WINAPI SE3DmaxExport::ExportWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (WM_INITDIALOG == msg)
	{
		SE3DmaxExport* instance = reinterpret_cast<SE3DmaxExport*>(lparam);
		assert(instance != NULL);

		::SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));

		int const normal_index = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("Normal"))));
		int const tangent_index = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("Tangent"))));
		int const texcoord_index = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("Texture coordinate"))));

		int const binormal_index = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(TEXT("Binormal"))));

		::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_SETITEMDATA, normal_index, static_cast<LPARAM>(ET_Normal));
		::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_SETITEMDATA, tangent_index, static_cast<LPARAM>(ET_Tangent));
		::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_SETITEMDATA, texcoord_index, static_cast<LPARAM>(ET_TextureCoord));

		::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_SETITEMDATA, binormal_index, static_cast<LPARAM>(ET_Binormal));

		::SendMessage(::GetDlgItem(wnd, IDC_COMBINE_MESHES), BM_SETCHECK, BST_CHECKED, NULL);
	}
	else
	{
		SE3DmaxExport* instance = reinterpret_cast<SE3DmaxExport*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg)
		{
		case WM_COMMAND:
			switch (LOWORD(wparam))
			{
			case IDC_BUTTON_ADD:
				{
					int const count = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_GETSELCOUNT, 0, 0));
					if (count > 0)
					{
						std::vector<int> sel_items(count);
						::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_GETSELITEMS, count, reinterpret_cast<LPARAM>(&sel_items[0]));

						for (size_t i = 0; i < sel_items.size(); ++ i)
						{
							int len = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_GETTEXTLEN, sel_items[i], NULL));
							std::vector<TCHAR> text(len + 1);
							::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_GETTEXT, sel_items[i], reinterpret_cast<LPARAM>(&text[0]));
							int data = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_GETITEMDATA, sel_items[i], NULL));

							int const ind = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(&text[0])));
							::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_SETITEMDATA, ind, static_cast<LPARAM>(data));

							::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_DELETESTRING, sel_items[i], 0);
						}
					}
				}
				break;

			case IDC_BUTTON_DEL:
				{
					int const count = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_GETSELCOUNT, 0, 0));
					if (count > 0)
					{
						std::vector<int> sel_items(count);
						::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_GETSELITEMS, count, reinterpret_cast<LPARAM>(&sel_items[0]));

						for (size_t i = 0; i < sel_items.size(); ++ i)
						{
							int len = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_GETTEXTLEN, sel_items[i], NULL));
							std::vector<TCHAR> text(len + 1);
							::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_GETTEXT, sel_items[i], reinterpret_cast<LPARAM>(&text[0]));
							int data = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_GETITEMDATA, sel_items[i], NULL));

							int const ind = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(&text[0])));
							::SendMessage(::GetDlgItem(wnd, IDC_LIST_IGNORED_VERTEX_ATTRS), LB_SETITEMDATA, ind, static_cast<LPARAM>(data));

							::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_DELETESTRING, sel_items[i], 0);
						}
					}
				}
				break;

			case IDOK:
				{
					assert(instance != NULL);

					int const joint_per_ver = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_SPIN_JOINT_PER_VER), UDM_GETPOS32, NULL, NULL));
					bool const combine_meshes = (BST_CHECKED == ::SendMessage(::GetDlgItem(wnd, IDC_COMBINE_MESHES), BM_GETCHECK, NULL, NULL));
					Interval const se_ticks = instance->m_pInterface->GetAnimRange();
					SE3DXMLExport extractor(instance->m_pInterface->GetRootNode(), joint_per_ver,
						instance->m_pInterface->GetTime(),
						se_ticks.Start() / GetTicksPerFrame(), se_ticks.End() / GetTicksPerFrame(),
						combine_meshes);

					SExportAttrs eva;
					eva.normal = false;
					eva.tangent = false;
					eva.binormal = false;
					eva.tex = false;
					for (int index = 0;; ++ index)
					{
						int data = static_cast<int>(::SendMessage(::GetDlgItem(wnd, IDC_LIST_EXPORT_VERTEX_ATTRS), LB_GETITEMDATA, index, NULL));
						if (data == LB_ERR)
						{
							break;
						}

						switch (data)
						{
						case ET_Normal:
							eva.normal = true;
							break;

						case ET_Tangent:
							eva.tangent = true;
							break;

						case ET_Binormal:
							eva.binormal = true;
							break;

						case ET_TextureCoord:
							eva.tex = true;
							break;
						}
					}

					extractor.ExportObjects(instance->m_ExportNodes);
					extractor.WriteXml(instance->m_sFileName, eva);

					::EndDialog(wnd, 1);
				}
				break;

			case IDCANCEL:
				::EndDialog(wnd, 0);
				break;
			}
			break;

		case WM_CLOSE:
			::EndDialog(wnd, 0);
			break;

		default:
			return FALSE;
		}
	}

	return TRUE;
}