//#ifndef _XMLDOM_HPP
//#define _XMLDOM_HPP
//
//#include <iosfwd>
//#include <vector>
//#include <SE3DEngine/Comm/Def.h>
//#include <SE3DEngine/ResourceSystem/DataStream.h>
//
//namespace SE
//{
//	enum EXmlNodeType
//	{
//		XNT_Document,
//		XNT_Element,
//		XNT_Data,
//		XNT_CData,
//		XNT_Comment,
//		XNT_Declaration,
//		XNT_Doctype,
//		XNT_PI
//	};
//
//	class  XmlDoc
//	{
//	public:
//		XmlDoc();
//
//		XmlNodePtr Parse(DataStreamPtr const & pSource);
//		void Print(std::ostream& os);
//
//		XmlNodePtr CloneNode(XmlNodePtr const & pNode);
//
//		XmlNodePtr AllocNode(EXmlNodeType eType, String const & sName);
//		XmlAttributePtr AllocAttribInt(String const & sName, int32 iValue);
//		XmlAttributePtr AllocAttribUInt(String const & sName, uint32 uValue);
//		XmlAttributePtr AllocAttribFloat(String const & sName, float fValue);
//		XmlAttributePtr AllocAttribString(String const & sName, String const & sValue);
//
//		void RootNode(XmlNodePtr const & pNewNode);
//
//	private:
//		boost::shared_ptr<void> m_pDoc;
//		std::vector<char> m_cXmlSrcs;
//
//		XmlNodePtr m_pNodeRoot;
//	};
//
//	class  XmlNode
//	{
//		friend class XmlDoc	;
//
//	public:
//		explicit XmlNode(void* node);
//		XmlNode(void* doc, EXmlNodeType type, String const & sName);
//
//		String const & Name() const;
//		EXmlNodeType Type() const;
//
//		XmlNodePtr Parent();
//
//		XmlAttributePtr FirstAttrib(String const & sName);
//		XmlAttributePtr LastAttrib(String const & sName);
//		XmlAttributePtr FirstAttrib();
//		XmlAttributePtr LastAttrib();
//
//		XmlAttributePtr Attrib(String const & sName);
//		int32 AttribInt(String const & sName, int32 iDefault_val);
//		uint32 AttribUInt(String const & sName, uint32 uDefault_val);
//		float AttribFloat(String const & sName, float fDefault_val);
//		String AttribString(String const & sName, String sDefault_val);
//
//		XmlNodePtr FirstNode(String const & sName);
//		XmlNodePtr LastNode(String const & sName);
//		XmlNodePtr FirstNode();
//		XmlNodePtr LastNode();
//
//		XmlNodePtr PrevSibling(String const & sName);
//		XmlNodePtr NextSibling(String const & sName);
//		XmlNodePtr PrevSibling();
//		XmlNodePtr NextSibling();
//
//		void InsertNode(XmlNodePtr const & location, XmlNodePtr const & pNewNode);
//		void InsertAttrib(XmlAttributePtr const & location, XmlAttributePtr const & pNewAttr);
//		void AppendNode(XmlNodePtr const & pNewNode);
//		void AppendAttrib(XmlAttributePtr const & pNewAttr);
//
//		void RemoveNode(XmlNodePtr const & pNode);
//		void RemoveAttrib(XmlAttributePtr const & pAttr);
//
//		int32  ValueInt() const;
//		uint32  ValueUInt() const;
//		float ValueFloat() const;
//		String ValueString() const;
//
//	private:
//		void* m_pNode;
//		String m_sName;
//
//		std::vector<XmlNodePtr> m_pChildrens;
//		std::vector<XmlAttributePtr> m_pAttrs;
//	};
//
//	class  XmlAttribute
//	{
//		friend class XmlDoc;
//		friend class XmlNode;
//
//	public:
//		explicit XmlAttribute(void* attr);
//		XmlAttribute(void* doc, String const & sName, String const & sValue);
//
//		String const & Name() const;
//
//		XmlAttributePtr NextAttrib(String const & sName);
//		XmlAttributePtr NextAttrib();
//
//		int32  ValueInt() const;
//		uint32  ValueUInt() const;
//		float ValueFloat() const;
//		String const & ValueString() const;
//
//	private:
//		void* m_pAttr;
//		String m_sName;
//		String m_sValue;
//	};
//}
//
//#endif		// _XMLDOM_HPP
