#include <SE3DEngine/ResourceSystem/CDataStream.h>



namespace SE
{
	CFileDataStream::CFileDataStream(String const& sName, uint16 eAcces)
	                 :m_sName(sName),m_eAcces(eAcces),m_uSize(0)
	{
	}
	CFileDataStream::~CFileDataStream()
	{
	}
	void CFileDataStream::SetName(String const& cpName)
	{
		m_sName = cpName;
	}
	String const& CFileDataStream::GetName() const
	{
		return m_sName;
	}
	uint16 CFileDataStream::GetAccessMode()const
	{
		return m_eAcces;
	}
	void CFileDataStream::SetAccessMode(uint16 const& eAcces)
	{
		m_eAcces = eAcces;
	}


	void CFileDataStream::Read(void* pBuf,size_t uSize)
	{
		  m_pIstream->read(static_cast<char*>(pBuf), static_cast<std::streamsize>(uSize));
	}

	int64 CFileDataStream::GetCount() const
	{
		return static_cast<int64>(m_pIstream->gcount());
	}

	void CFileDataStream::Seekg(int64 iOffset, std::ios_base::seekdir way)
	{
		m_pIstream->seekg(static_cast<std::istream::off_type>(iOffset), way);
	}

	int64 CFileDataStream::Tellg()
	{
		return static_cast<int64>(m_pIstream->tellg());
	}

	void CFileDataStream::Clear()
	{
		m_pIstream->clear();
	}

}