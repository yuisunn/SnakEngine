#include <SE3DEngine/ResourceSystem/DataStreamFile.h>

namespace SE
{
    void DataStreamFile::Open()
	{
		if (('/' == name[0]) || ('\\' == name[0]))
		{
			if (boost::filesystem::exists(name))
			{
				uint64_t timestamp = boost::filesystem::last_write_time(name);
				return MakeSharedPtr<ResIdentifier>(name, timestamp,
					MakeSharedPtr<std::ifstream>(name.c_str(), std::ios_base::binary));
			}
		}
		else
		{
			typedef BOOST_TYPEOF(pathes_) PathesType;
			BOOST_FOREACH(PathesType::const_reference path, pathes_)
			{
				std::string const res_name(path + name);

				if (boost::filesystem::exists(res_name))
				{
					uint64_t timestamp = boost::filesystem::last_write_time(res_name);
					return MakeSharedPtr<ResIdentifier>(name, timestamp,
						MakeSharedPtr<std::ifstream>(res_name.c_str(), std::ios_base::binary));
				}
				else
				{
					std::string::size_type const pkt_offset(res_name.find("//"));
					if (pkt_offset != std::string::npos)
					{
						std::string pkt_name = res_name.substr(0, pkt_offset);
						if (boost::filesystem::exists(pkt_name)
								&& (boost::filesystem::is_regular_file(pkt_name)
										|| boost::filesystem::is_symlink(pkt_name)))
						{
							std::string::size_type const password_offset = pkt_name.find("|");
							std::string password;
							if (password_offset != std::string::npos)
							{
								password = pkt_name.substr(password_offset + 1);
								pkt_name = pkt_name.substr(0, password_offset - 1);
							}
							std::string const file_name = res_name.substr(pkt_offset + 2);

							uint64_t timestamp = boost::filesystem::last_write_time(pkt_name);
							ResIdentifierPtr pkt_file = MakeSharedPtr<ResIdentifier>(name, timestamp,
								MakeSharedPtr<std::ifstream>(pkt_name.c_str(), std::ios_base::binary));
							if (*pkt_file)
							{
								boost::shared_ptr<std::iostream> packet_file = MakeSharedPtr<std::stringstream>();
								Extract7z(pkt_file, password, file_name, packet_file);
								return MakeSharedPtr<ResIdentifier>(name, timestamp, packet_file);
							}
						}
					}
				}
			}
		}

		return ResIdentifierPtr();

	}
	uint16 DataStreamFile::GetAcessMode()const
	{

	}
	void DataStreamFile::Seekg(int64 iOffset, std::ios_base::seekdir way)
	{
		m_pIstream->seekg(static_cast<std::istream::off_type>(iOffset), way);
	}
	int64 DataStreamFile::Tellg()
	{
		return static_cast<int64>(m_pIstream->tellg());
	}
	bool DataStreamFile::IsRead() const;
	bool DataStreamFile::IsWrite() const;
	size_t DataStreamFile::Read(void* pBuf,size_t size)
	{
		m_pIstream->read(static_cast<char*>(pBuf), static_cast<std::streamsize>(size));
	}
	size_t DataStreamFile::Write(const void* pBuf, size_t uCount)
	{
		
	}
	void DataStreamFile::Clear()
	{
		m_pIstream->clear();
	}


}