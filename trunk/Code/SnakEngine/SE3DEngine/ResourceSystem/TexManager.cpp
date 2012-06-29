#include <SE3DEngine/ResourceSystem/TexManager.h>
#include <boost/filesystem.hpp>
#include <boost/typeof/typeof.hpp>
 

namespace SE
{

	TexManager* TexManager::Instance()
    {
		if(!m_pInstance)
			m_pInstance = new TexManager();
		return m_pInstance;

	}

	void Locate(string const & name)
	{
		if (('/' == name[0]) || ('\\' == name[0]))
		{
			if (boost::filesystem::exists(name))
			{
				return name;
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
					return res_name;
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
								if (Find7z(pkt_file, password, file_name) != 0xFFFFFFFF)
								{
									return res_name;
								}
							}
						}
					}
				}
			}
		}

		return "";
	}

	void TexManager::AddPath(String const & sPath)
	{
		/*boost::filesystem::path new_path(path);
		if (!new_path.is_complete())
		{
			boost::filesystem::path full_path = exe_path_ / new_path;
			if (!boost::filesystem::exists(full_path))
			{
				full_path = boost::filesystem::current_path() / new_path;
				if (!boost::filesystem::exists(full_path))
				{
					return;
				}
			}
			new_path = full_path;
		}
		String path_str = new_path.string();
		if (path_str[path_str.length() - 1] != '/')
		{
			path_str.push_back('/');
		}
		m_sPathes.push_back(path_str);*/
	}

	bool TexManager::ResExists(String const &sName)
	{
		// C++里面不会有这种运行时的特性，C++标准委员会的那一帮偏执狂绝对不会允许任何可能降低效率的特性的……所以BOOST_AUTO其实是一个编译时的推算，在运行的时候，那个x就是一个已经确定的int。

        //BOOST_AUTO(x, 1);也可以写成BOOST_TYPEOF(1) x = 1; 其含义一目了然。
        //c++0x 正佳关键字 
			auto iter =  m_texMaps.find(sName);
			if (m_texMaps.end() == iter)
			{
				return false;
			}
			else
			{
				return true;
			}


		
	}
}