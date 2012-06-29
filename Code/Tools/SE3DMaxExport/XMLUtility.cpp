
#include "XMLUtility.h"

#pragma comment(lib, "msxml2.lib") 


	std::string tstr_to_str(std::basic_string<TCHAR> const & tstr)
	{
	#ifdef UNICODE
		std::vector<char> temp(wcstombs(NULL, tstr.c_str(), 0));
		wcstombs(&temp[0], tstr.c_str(), temp.size());
		return std::string(temp.begin(), temp.end());
	#else
		return tstr;
	#endif
	}

	bool IsMesh(INode* node)
	{
		if (NULL == node)
		{
			return false;
		}

		ObjectState os = node->EvalWorldState(0);
		if (NULL == os.obj)
		{
			return false;
		}

		if (os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID)
		{
			if (os.obj->ClassID() != Class_ID(TARGET_CLASS_ID, 0))
			{
				return true;
			}
		}
		
		return false;
	}

	bool IsBone(INode* node)
	{
		if (NULL == node)
		{
			return false;
		}

		ObjectState os = node->EvalWorldState(0);
		if (NULL == os.obj)
		{
			return false;
		}

		if (os.obj->SuperClassID() == HELPER_CLASS_ID)
		{
			return true;
		}

		if (os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID)
		{
			if (os.obj->ClassID() == BONE_OBJ_CLASSID)
			{
				return true;
			}
		}

		Control* ctl = node->GetTMController();
		if ((ctl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID)
			|| (ctl->ClassID() == BIPBODY_CONTROL_CLASS_ID))
		{
			return true;
		}

		return false;
	}

//IXMLDOMNode对象代表XML文档树形结构中的一个单个节点，它可能包含其他子节点的元素。该对象描述XML文档里的节点。这个节点可以是元素、属性、处理指令、文本或者其他存储在XML文档里的信息。
//IXMLDOMDocument 一个mxl 文件对象？？
//附加对象 
//newNode？
//bool CreateXMLNode(CComPtr<IXMLDOMDocument> doc, CComPtr<IXMLDOMNode> node, const TCHAR *nodeName, IXMLDOMNode ** newNode)
//{
//
//	CComPtr <IXMLDOMNode> sceneNode;
//	doc->createNode(CComVariant(NODE_ELEMENT), CComBSTR(nodeName), NULL, &sceneNode);
//	node->appendChild(sceneNode, newNode);
//	return true;
//}
//
//bool AddXMLAttribute(CComPtr<IXMLDOMNode> node, const TCHAR * name, const TCHAR * value)
//{
//	CComQIPtr<IXMLDOMElement> element;
//	element = node;
//	element->setAttribute(CComBSTR(name), CComVariant(value));
//	return true;
//}
////节点套节点？
//bool AddXMLText(CComPtr<IXMLDOMDocument> doc, CComPtr<IXMLDOMNode> node, const TCHAR * text)
//{
//	CComPtr <IXMLDOMText> keydata;
//	doc->createTextNode(CComBSTR(text), &keydata);
//	node->appendChild(keydata,NULL);
//	return true;
//
//}
//CComPtr<IXMLDOMText> CreateXMLText(CComPtr<IXMLDOMDocument> doc, CComPtr<IXMLDOMNode> parentNode, const TCHAR* name)
//{
//	CComPtr<IXMLDOMNode> node;
//	CComPtr<IXMLDOMText> textNode;
//
//	if( CreateXMLNode(doc,parentNode, name,&node) == true) {
//		doc->createTextNode(CComBSTR(""), &textNode);
//		node->appendChild(textNode,NULL);
//	}
//
//	return textNode;
//}
//
//void PrettyPrint(const TCHAR* name, CComPtr<IXMLDOMDocument> pXMLDoc)
//{
//	// perform formatting XSLT transform to get indented XML output
//	CComPtr<IXMLDOMDocument> pXSLDoc;
//	BSTR outputXML = NULL;
//	HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,  IID_IXMLDOMDocument, (void**)&pXSLDoc);
//	if (SUCCEEDED(hr)) {
//		// load indenting XSL doc 
//		VARIANT_BOOL result;
//		CComBSTR indentXSL(
//			"<xsl:stylesheet version=\"1.0\""
//			"      xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
//			"   <xsl:output method=\"xml\"/>"
//			"   <xsl:param name=\"indent-increment\" select=\"'\t'\" />"
//			"   <xsl:template match=\"node()\">"
//			"      <xsl:param name=\"indent\" select=\"'&#xA;'\"/>"
//			"      <xsl:value-of select=\"$indent\"/>"
//			"      <xsl:copy>"
//			"        <xsl:copy-of select=\"@*\" />"
//			"        <xsl:apply-templates>"
//			"          <xsl:with-param name=\"indent\""
//			"               select=\"concat($indent, $indent-increment)\"/>"
//			"        </xsl:apply-templates>"
//			"        <xsl:if test=\"node()\">"
//			"          <xsl:value-of select=\"$indent\"/>"
//			"        </xsl:if>"
//			"      </xsl:copy>"
//			"   </xsl:template>"
////			"   <xsl:template match=\"comment()|processing-instruction()\">"
////			"      <xsl:copy />"
////			"   </xsl:template>"
////			"   <!-- WARNING: this is dangerous. Handle with care -->"
////			"   <xsl:template match=\"text()[normalize-space(.)='']\"/>"
//			"</xsl:stylesheet>"
//			);
//		hr = pXSLDoc->loadXML(indentXSL, &result);
//		if (SUCCEEDED(hr)) {
//			// perform transform
//			hr = pXMLDoc->transformNode(pXSLDoc, &outputXML);
//		}
//	}
//
//	// output transformed XML if previous sequence succeeded, else normal XMLDoc save
//	if (SUCCEEDED(hr)) {
//		FILE* out = _tfopen(name, _T("w"));
//		if (out != NULL) {
//			// hack the UTF-16 back to UTF-8 (there probably is a way to mod the stylesheet to do this)
//			wchar_t* enc = wcsstr(outputXML, L"\"UTF-16\"");
//			if (enc != NULL) memcpy(enc, L"\"utf-8\" ", 8 * sizeof(wchar_t));
//			// convert BSTR to MBCS for output
//			int len = WideCharToMultiByte(CP_ACP, 0, outputXML, -1, 0, 0, NULL, NULL);
//			TCHAR* buf = (TCHAR*)malloc(len * sizeof(WCHAR));
//			WideCharToMultiByte(CP_ACP, 0, outputXML, -1, buf, len, NULL, NULL);
//			// write the XML
//			_fputts(buf, out);   
//			fclose(out);
//			free(buf);
//		}
//		SysFreeString(outputXML);
//	}
//	else
//		// save the XML graph out to the export file
//		pXMLDoc->save(CComVariant(name));
//
//	
//}
