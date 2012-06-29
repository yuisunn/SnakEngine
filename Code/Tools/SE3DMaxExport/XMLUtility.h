
#ifndef _XMLUtity_H_
#define _XMLUtity_H_
#include <max.h>
#if VERSION_3DSMAX >= 7 << 16
#include <CS/bipexp.h>
#else
#include <bipexp.h>
#endif
#include <string>

std::string tstr_to_str(std::basic_string<TCHAR> const & tstr);

bool IsMesh(INode* node);
bool IsBone(INode* node);


//bool CreateXMLNode(CComPtr<IXMLDOMDocument> doc, CComPtr<IXMLDOMNode> node, const TCHAR *nodeName, IXMLDOMNode **  newNode);
//bool AddXMLAttribute(CComPtr<IXMLDOMNode> node, const TCHAR * name, const TCHAR * value);
//bool AddXMLText(CComPtr<IXMLDOMDocument> doc, CComPtr<IXMLDOMNode> node, const TCHAR * text);
//CComPtr<IXMLDOMText> CreateXMLText(CComPtr<IXMLDOMDocument>, CComPtr<IXMLDOMNode>, const TCHAR*);
//void PrettyPrint(const TCHAR*, CComPtr<IXMLDOMDocument> );
//

#endif