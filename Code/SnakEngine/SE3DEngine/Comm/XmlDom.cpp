 
 

 

#include <sstream>
 
 
#pragma warning(disable: 4702)
 
#include <boost/lexical_cast.hpp>
 
 

 
#pragma warning(disable: 4100 6011 6313)
 
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <SE3DEngine/Comm/Util.h>
 
#include <SE3DEngine/Comm/XmlDom.h>

namespace SE
{
	XmlDoc::XmlDoc()
		: m_pDoc(MakeSharedPtr<rapidxml::xml_document<> >())
	{
	}

	XmlNodePtr XmlDoc::Parse(DataStreamPtr const & pSource)
	{
		pSource->Seekg(0, std::ios_base::end);
		int len = static_cast<int>(pSource->Tellg());
		pSource->Seekg(0, std::ios_base::beg);
		m_cXmlSrcs.resize(len + 1, 0);
		pSource->Read(&m_cXmlSrcs[0], len);

		static_cast<rapidxml::xml_document<>*>(m_pDoc.get())->parse<0>(&m_cXmlSrcs[0]);
		m_pNodeRoot = MakeSharedPtr<XmlNode>(static_cast<rapidxml::xml_document<>*>(m_pDoc.get())->first_node());

		return m_pNodeRoot;
	}

	void XmlDoc::Print(std::ostream& os)
	{
		os << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
		os << *static_cast<rapidxml::xml_document<>*>(m_pDoc.get());
	}

	XmlNodePtr XmlDoc::CloneNode(XmlNodePtr const & pNode)
	{
		return MakeSharedPtr<XmlNode>(static_cast<rapidxml::xml_document<>*>(m_pDoc.get())->clone_node(static_cast<rapidxml::xml_node<>*>(pNode->m_pNode)));
	}

	XmlNodePtr XmlDoc::AllocNode(EXmlNodeType eType, String const & sName)
	{
		return MakeSharedPtr<XmlNode>(m_pDoc.get(), eType, sName);
	}
	
	XmlAttributePtr XmlDoc::AllocAttribInt(String const & sName, int32 iValue)
	{
		return this->AllocAttribString(sName, boost::lexical_cast<String>(iValue));
	}

	XmlAttributePtr XmlDoc::AllocAttribUInt(String const & sName, uint32 uValue)
	{
		return this->AllocAttribString(sName, boost::lexical_cast<String>(uValue));
	}

	XmlAttributePtr XmlDoc::AllocAttribFloat(String const & sName, float fValue)
	{
		return this->AllocAttribString(sName, boost::lexical_cast<String>(fValue));
	}

	XmlAttributePtr XmlDoc::AllocAttribString(String const & sName, String const & sValue)
	{
		return MakeSharedPtr<XmlAttribute>(m_pDoc.get(), sName, sValue);
	}

	void XmlDoc::RootNode(XmlNodePtr const & pNewNode)
	{
		static_cast<rapidxml::xml_document<>*>(m_pDoc.get())->remove_all_nodes();
		static_cast<rapidxml::xml_document<>*>(m_pDoc.get())->append_node(static_cast<rapidxml::xml_node<>*>(pNewNode->m_pNode));
		m_pNodeRoot = pNewNode;
	}

}

namespace SE
{
	//node
	XmlNode::XmlNode(void* pNode)
		: m_pNode(pNode)
	{
		if (m_pNode != NULL)
		{
			m_sName = String(static_cast<rapidxml::xml_node<>*>(m_pNode)->name(),
				static_cast<rapidxml::xml_node<>*>(m_pNode)->name_size());
		}
	}

	XmlNode::XmlNode(void* doc, EXmlNodeType eType, String const & sName)
		: m_sName(sName)
	{
		rapidxml::node_type xtype;
		switch (eType)
		{
		case XNT_Document:
			xtype = rapidxml::node_document;
			break;

		case XNT_Element:
			xtype = rapidxml::node_element;
			break;

		case XNT_Data:
			xtype = rapidxml::node_data;
			break;

		case XNT_CData:
			xtype = rapidxml::node_cdata;
			break;

		case XNT_Comment:
			xtype = rapidxml::node_comment;
			break;

		case XNT_Declaration:
			xtype = rapidxml::node_declaration;
			break;

		case XNT_Doctype:
			xtype = rapidxml::node_doctype;
			break;

		case XNT_PI:
		default:
			xtype = rapidxml::node_pi;
			break;
		}

		m_pNode = static_cast<rapidxml::xml_document<>*>(doc)->allocate_node(xtype, m_sName.c_str());
	}

	String const & XmlNode::Name() const
	{
		return m_sName;
	}

	EXmlNodeType XmlNode::Type() const
	{
		switch (static_cast<rapidxml::xml_node<>*>(m_pNode)->type())
		{
		case rapidxml::node_document:
			return XNT_Document;

		case rapidxml::node_element:
			return XNT_Element;

		case rapidxml::node_data:
			return XNT_Data;

		case rapidxml::node_cdata:
			return XNT_CData;

		case rapidxml::node_comment:
			return XNT_Comment;

		case rapidxml::node_declaration:
			return XNT_Declaration;

		case rapidxml::node_doctype:
			return XNT_Doctype;

		case rapidxml::node_pi:
		default:
			return XNT_PI;
		}
	}

    XmlNodePtr XmlNode::Parent()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->parent();
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlAttributePtr XmlNode::FirstAttrib(String const & sName)
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(m_pNode)->first_attribute(m_sName.c_str());
		if (attr)
		{
			return MakeSharedPtr<XmlAttribute>(attr);
		}
		else
		{
			return XmlAttributePtr();
		}
	}
	
	XmlAttributePtr XmlNode::LastAttrib(String const & sName)
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(m_pNode)->last_attribute(sName.c_str());
		if (attr)
		{
			return MakeSharedPtr<XmlAttribute>(attr);
		}
		else
		{
			return XmlAttributePtr();
		}
	}

	XmlAttributePtr XmlNode::FirstAttrib()
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(m_pNode)->first_attribute();
		if (attr)
		{
			return MakeSharedPtr<XmlAttribute>(attr);
		}
		else
		{
			return XmlAttributePtr();
		}
	}

	XmlAttributePtr XmlNode::LastAttrib()
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_node<>*>(m_pNode)->last_attribute();
		if (attr)
		{
			return MakeSharedPtr<XmlAttribute>(attr);
		}
		else
		{
			return XmlAttributePtr();
		}
	}

	XmlAttributePtr XmlNode::Attrib(String const & sName)
	{
		return this->FirstAttrib(sName);
	}

	int32 XmlNode::AttribInt(String const & sName, int32 iDefault_val)
	{
		XmlAttributePtr attr = this->Attrib(sName);
		return attr ? attr->ValueInt() : iDefault_val;
	}

	uint32 XmlNode::AttribUInt(String const & sName, uint32 uDefault_val)
	{
		XmlAttributePtr attr = this->Attrib(sName);
		return attr ? attr->ValueUInt() : uDefault_val;
	}

	float XmlNode::AttribFloat(String const & sName, float fDefault_val)
	{
		XmlAttributePtr attr = this->Attrib(sName);
		return attr ? attr->ValueFloat() : fDefault_val;
	}

	String XmlNode::AttribString(String const & sName, String sDefault_val)
	{
		XmlAttributePtr attr = this->Attrib(sName);
		return attr ? attr->ValueString() : sDefault_val;
	}

	XmlNodePtr XmlNode::FirstNode(String const & sName)
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->first_node(sName.c_str());
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::LastNode(String const & sName)
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->last_node(sName.c_str());
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::FirstNode()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->first_node();
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::LastNode()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->last_node();
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::PrevSibling(String const & sName)
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->previous_sibling(sName.c_str());
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::NextSibling(String const & sName)
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->next_sibling(sName.c_str());
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::PrevSibling()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->previous_sibling();
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	XmlNodePtr XmlNode::NextSibling()
	{
		rapidxml::xml_node<>* node = static_cast<rapidxml::xml_node<>*>(m_pNode)->next_sibling();
		if (node)
		{
			return MakeSharedPtr<XmlNode>(node);
		}
		else
		{
			return XmlNodePtr();
		}
	}

	void XmlNode::InsertNode(XmlNodePtr const & pLocation, XmlNodePtr const & pNewNode)
	{
		static_cast<rapidxml::xml_node<>*>(m_pNode)->insert_node(static_cast<rapidxml::xml_node<>*>(pLocation->m_pNode),
		static_cast<rapidxml::xml_node<>*>(pNewNode->m_pNode));
		for (size_t i = 0; i < m_pChildrens.size(); ++ i)
		{
			if (m_pChildrens[i]->m_pNode == pLocation->m_pNode)
			{
				m_pChildrens.insert(m_pChildrens.begin() + i, pNewNode);
				break;
			}
		}
	}

	void XmlNode::InsertAttrib(XmlAttributePtr const & pLocation, XmlAttributePtr const & pNewAttr)
	{
		static_cast<rapidxml::xml_node<>*>(m_pNode)->insert_attribute(static_cast<rapidxml::xml_attribute<>*>(pLocation->m_pAttr),
			static_cast<rapidxml::xml_attribute<>*>(pNewAttr->m_pAttr));
		for (size_t i = 0; i < m_pAttrs.size(); ++ i)
		{
			if (m_pAttrs[i]->m_pAttr == pLocation->m_pAttr)
			{
				m_pAttrs.insert(m_pAttrs.begin() + i, pNewAttr);
				break;
			}
		}
	}

	void XmlNode::AppendNode(XmlNodePtr const & pNewNode)
	{
		static_cast<rapidxml::xml_node<>*>(m_pNode)->append_node(static_cast<rapidxml::xml_node<>*>(pNewNode->m_pNode));
		m_pChildrens.push_back(pNewNode);
	}

	void XmlNode::AppendAttrib(XmlAttributePtr const & new_attr)
	{
	     static_cast<rapidxml::xml_node<>*>(m_pNode)->append_attribute(static_cast<rapidxml::xml_attribute<>*>(new_attr->m_pAttr));
		m_pAttrs.push_back(new_attr);
	}

	void XmlNode::RemoveNode(XmlNodePtr const & pNode)
	{
		static_cast<rapidxml::xml_node<>*>(m_pNode)->remove_node(static_cast<rapidxml::xml_node<>*>(pNode->m_pNode));
		for (size_t i = 0; i < m_pChildrens.size(); ++ i)
		{
			if (m_pChildrens[i]->m_pNode == pNode->m_pNode)
			{
				m_pChildrens.erase(m_pChildrens.begin() + i);
				break;
			}
		}
	}

	void XmlNode::RemoveAttrib(XmlAttributePtr const & pAttr)
	{
		static_cast<rapidxml::xml_node<>*>(m_pNode)->remove_attribute(static_cast<rapidxml::xml_attribute<>*>(pAttr->m_pAttr));
		for (size_t i = 0; i < m_pAttrs.size(); ++ i)
		{
			if (m_pAttrs[i]->m_pAttr == pAttr->m_pAttr)
			{
				m_pAttrs.erase(m_pAttrs.begin() + i);
				break;
			}
		}
	}

	int32 XmlNode::ValueInt() const
	{
		return boost::lexical_cast<int32>(this->ValueString());
	}

	uint32 XmlNode::ValueUInt() const
	{
		return boost::lexical_cast<uint32>(this->ValueString());
	}

	float XmlNode::ValueFloat() const
	{
		return boost::lexical_cast<float>(this->ValueString());
	}

	String XmlNode::ValueString() const
	{
		return String(static_cast<rapidxml::xml_node<>*>(m_pNode)->value(),
			static_cast<rapidxml::xml_node<>*>(m_pNode)->value_size());
	}


	XmlAttribute::XmlAttribute(void* pAttr)
		: m_pAttr(pAttr)
	{
		if (m_pAttr != NULL)
		{
			m_sName = String(static_cast<rapidxml::xml_attribute<>*>(m_pAttr)->name(), static_cast<rapidxml::xml_attribute<>*>(m_pAttr)->name_size());
			m_sValue = String(static_cast<rapidxml::xml_attribute<>*>(m_pAttr)->value(), static_cast<rapidxml::xml_attribute<>*>(m_pAttr)->value_size());
		}
	}

	XmlAttribute::XmlAttribute(void* pDoc, String const & sName, String const & sValue)
		: m_sName(sName), m_sValue(sValue)
	{
		m_pAttr = static_cast<rapidxml::xml_document<>*>(pDoc)->allocate_attribute(sName.c_str(), sValue.c_str());
	}

	String const & XmlAttribute::Name() const
	{
		return m_sName;
	}

	XmlAttributePtr XmlAttribute::NextAttrib(String const & sName)
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_attribute<>*>(m_pAttr)->next_attribute(sName.c_str());
		if (attr)
		{
			return MakeSharedPtr<XmlAttribute>(attr);
		}
		else
		{
			return XmlAttributePtr();
		}
	}

	XmlAttributePtr XmlAttribute::NextAttrib()
	{
		rapidxml::xml_attribute<>* attr = static_cast<rapidxml::xml_attribute<>*>(m_pAttr)->next_attribute();
		if (attr)
		{
			return MakeSharedPtr<XmlAttribute>(attr);
		}
		else
		{
			return XmlAttributePtr();
		}
	}

	int32 XmlAttribute::ValueInt() const
	{
		return boost::lexical_cast<int32>(m_sValue);
	}

	uint32 XmlAttribute::ValueUInt() const
	{
		return boost::lexical_cast<uint32>(m_sValue);
	}

	float XmlAttribute::ValueFloat() const
	{
		return boost::lexical_cast<float>(m_sValue);
	}

	String const & XmlAttribute::ValueString() const
	{
		return m_sValue;
	}
}
