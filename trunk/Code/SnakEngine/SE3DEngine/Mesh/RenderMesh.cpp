#include <SE3DEngine/Mesh/RenderMesh.h>
#include <SEMath/Vector2.h>
#include <iosfwd>
#include <SE3DEngine/Comm/XmlDom.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <boost/tuple/tuple.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <SE3DEngine/Comm/Util.h>

namespace SE
{	
	//void ReadShortString(ResIdentifierPtr& file, std::string& str)
	//{
	//	uint8 len;
	//	file->read(reinterpret_cast<char*>(&len), sizeof(len));
	//	str.resize(len);
	//	file->read(reinterpret_cast<char*>(&str[0]), len * sizeof(str[0]));
	//}

	void WriteShortString(std::ostream& os, std::string const & str)
	{
		BOOST_ASSERT(str.size() < 256);

		uint8 len = static_cast<uint8>(str.length());
		os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		os.write(reinterpret_cast<char const *>(&str[0]), str.size() * sizeof(str[0]));
	}

	void LoadMeshXml()
	{
		//二井之流
		std::shared_ptr<std::stringstream> ss = MakeSharedPtr<std::stringstream>();
		    XmlDoc doc;
			//文件读到内存
			XmlNodePtr root = doc.Parse(file);
			//定义的assert 宏
			BOOST_ASSERT(root->Attrib("version") && (root->Attrib("version")->ValueInt() >= 4));
			XmlNodePtr materials_chunk = root->FirstNode("materials_chunk");
			if (materials_chunk)
			{
				//活得material 数量
				uint32 num_mtls = 0;
				for (XmlNodePtr mtl_node = materials_chunk->FirstNode("material"); mtl_node; mtl_node = mtl_node->NextSibling("material"))
				{
					++ num_mtls;
				}
				//字节存储数据 低位存储 cpu 不同导致
				NativeToLittleEndian<sizeof(num_mtls)>(&num_mtls);
				//吧num 每天临睡写入到 流中
				ss->write(reinterpret_cast<char*>(&num_mtls), sizeof(num_mtls));
			}
			else
			{
				uint32 num_mtls = 0;
				NativeToLittleEndian<sizeof(num_mtls)>(&num_mtls);
				ss->write(reinterpret_cast<char*>(&num_mtls), sizeof(num_mtls));
			}

			XmlNodePtr meshes_chunk = root->FirstNode("meshes_chunk");
			if (meshes_chunk)
			{
				uint32 num_meshes = 0;
				for (XmlNodePtr mesh_node = meshes_chunk->FirstNode("mesh"); mesh_node; mesh_node = mesh_node->NextSibling("mesh"))
				{
					++ num_meshes;
				}
				NativeToLittleEndian<sizeof(num_meshes)>(&num_meshes);
				ss->write(reinterpret_cast<char*>(&num_meshes), sizeof(num_meshes));
			}
			else
			{
				uint32 num_meshes = 0;
				NativeToLittleEndian<sizeof(num_meshes)>(&num_meshes);
				ss->write(reinterpret_cast<char*>(&num_meshes), sizeof(num_meshes));
			}

			XmlNodePtr bones_chunk = root->FirstNode("bones_chunk");
			if (bones_chunk)
			{
				uint32 num_joints = 0;
				for (XmlNodePtr bone_node = bones_chunk->FirstNode("bone"); bone_node; bone_node = bone_node->NextSibling("bone"))
				{
					++ num_joints;
				}
				NativeToLittleEndian<sizeof(num_joints)>(&num_joints);
				ss->write(reinterpret_cast<char*>(&num_joints), sizeof(num_joints));
			}
			else
			{
				uint32 num_joints = 0;
				NativeToLittleEndian<sizeof(num_joints)>(&num_joints);
				ss->write(reinterpret_cast<char*>(&num_joints), sizeof(num_joints));
			}

			XmlNodePtr key_frames_chunk = root->FirstNode("key_frames_chunk");
			if (key_frames_chunk)
			{
				uint32 num_kfs = 0;
				for (XmlNodePtr kf_node = key_frames_chunk->FirstNode("key_frame"); kf_node; kf_node = kf_node->NextSibling("key_frame"))
				{
					++ num_kfs;
				}
				NativeToLittleEndian<sizeof(num_kfs)>(&num_kfs);
				ss->write(reinterpret_cast<char*>(&num_kfs), sizeof(num_kfs));
			}
			else
			{
				uint32 num_kfs = 0;
				NativeToLittleEndian<sizeof(num_kfs)>(&num_kfs);
				ss->write(reinterpret_cast<char*>(&num_kfs), sizeof(num_kfs));
			}

			if (materials_chunk)
			{
				uint32 mtl_index = 0;
				for (XmlNodePtr mtl_node = materials_chunk->FirstNode("material"); mtl_node; mtl_node = mtl_node->NextSibling("material"), ++ mtl_index)
				{
					SRenderMaterialPtr mtl = MakeSharedPtr<SRenderMaterial>();
					float ambient_r = mtl_node->Attrib("ambient_r")->ValueFloat();
					float ambient_g = mtl_node->Attrib("ambient_g")->ValueFloat();
					float ambient_b = mtl_node->Attrib("ambient_b")->ValueFloat();
					float diffuse_r = mtl_node->Attrib("diffuse_r")->ValueFloat();
					float diffuse_g = mtl_node->Attrib("diffuse_g")->ValueFloat();
					float diffuse_b = mtl_node->Attrib("diffuse_b")->ValueFloat();
					float specular_r = mtl_node->Attrib("specular_r")->ValueFloat();
					float specular_g = mtl_node->Attrib("specular_g")->ValueFloat();
					float specular_b = mtl_node->Attrib("specular_b")->ValueFloat();
					float emit_r = mtl_node->Attrib("emit_r")->ValueFloat();
					float emit_g = mtl_node->Attrib("emit_g")->ValueFloat();
					float emit_b = mtl_node->Attrib("emit_b")->ValueFloat();
					float opacity = mtl_node->Attrib("opacity")->ValueFloat();
					float specular_level = mtl_node->Attrib("specular_level")->ValueFloat();
					float shininess = mtl_node->Attrib("shininess")->ValueFloat();

					NativeToLittleEndian<sizeof(ambient_r)>(&ambient_r);
					NativeToLittleEndian<sizeof(ambient_g)>(&ambient_g);
					NativeToLittleEndian<sizeof(ambient_b)>(&ambient_b);
					NativeToLittleEndian<sizeof(diffuse_r)>(&diffuse_r);
					NativeToLittleEndian<sizeof(diffuse_g)>(&diffuse_g);
					NativeToLittleEndian<sizeof(diffuse_b)>(&diffuse_b);
					NativeToLittleEndian<sizeof(specular_r)>(&specular_r);
					NativeToLittleEndian<sizeof(specular_g)>(&specular_g);
					NativeToLittleEndian<sizeof(specular_b)>(&specular_b);
					NativeToLittleEndian<sizeof(emit_r)>(&emit_r);
					NativeToLittleEndian<sizeof(emit_g)>(&emit_g);
					NativeToLittleEndian<sizeof(emit_b)>(&emit_b);
					NativeToLittleEndian<sizeof(opacity)>(&opacity);
					NativeToLittleEndian<sizeof(specular_level)>(&specular_level);
					NativeToLittleEndian<sizeof(shininess)>(&shininess);

					ss->write(reinterpret_cast<char*>(&ambient_r), sizeof(ambient_r));
					ss->write(reinterpret_cast<char*>(&ambient_g), sizeof(ambient_g));
					ss->write(reinterpret_cast<char*>(&ambient_b), sizeof(ambient_b));
					ss->write(reinterpret_cast<char*>(&diffuse_r), sizeof(diffuse_r));
					ss->write(reinterpret_cast<char*>(&diffuse_g), sizeof(diffuse_g));
					ss->write(reinterpret_cast<char*>(&diffuse_b), sizeof(diffuse_b));
					ss->write(reinterpret_cast<char*>(&specular_r), sizeof(specular_r));
					ss->write(reinterpret_cast<char*>(&specular_g), sizeof(specular_g));
					ss->write(reinterpret_cast<char*>(&specular_b), sizeof(specular_b));
					ss->write(reinterpret_cast<char*>(&emit_r), sizeof(emit_r));
					ss->write(reinterpret_cast<char*>(&emit_g), sizeof(emit_g));
					ss->write(reinterpret_cast<char*>(&emit_b), sizeof(emit_b));
					ss->write(reinterpret_cast<char*>(&opacity), sizeof(opacity));
					ss->write(reinterpret_cast<char*>(&specular_level), sizeof(specular_level));
					ss->write(reinterpret_cast<char*>(&shininess), sizeof(shininess));

					XmlNodePtr textures_chunk = mtl_node->FirstNode("textures_chunk");
					if (textures_chunk)
					{
						uint32 num_texs = 0;
						for (XmlNodePtr tex_node = textures_chunk->FirstNode("texture"); tex_node; tex_node = tex_node->NextSibling("texture"))
						{
							++ num_texs;
						}
						NativeToLittleEndian<sizeof(num_texs)>(&num_texs);
						ss->write(reinterpret_cast<char*>(&num_texs), sizeof(num_texs));

						for (XmlNodePtr tex_node = textures_chunk->FirstNode("texture"); tex_node; tex_node = tex_node->NextSibling("texture"))
						{
							WriteShortString(*ss, tex_node->Attrib("type")->ValueString());
							WriteShortString(*ss, tex_node->Attrib("name")->ValueString());
						}
					}
					else
					{
						uint32 num_texs = 0;
						NativeToLittleEndian<sizeof(num_texs)>(&num_texs);
						ss->write(reinterpret_cast<char*>(&num_texs), sizeof(num_texs));
					}
				}
			}

			if (meshes_chunk)
			{
				RenderFactory& rf = Context::Instance().RenderFactoryInstance();
				EColorFormat tbn_format;
				if (rf.RenderEngineInstance().DeviceCaps().m_eVertexFormatSupport(CF_A2RGB10))
				{
					tbn_format = CF_A2RGB10;
				}
				else
				{
					BOOST_ASSERT(rf.RenderEngineInstance().DeviceCaps().m_eVertexFormatSupport(CF_ARGB8));

					tbn_format = CF_ARGB8;
				}

				std::vector<std::vector<vertex_element> > ves;
				std::vector<uint32> mesh_num_vertices;
				std::vector<uint32> mesh_base_vertices(1, 0);
				std::vector<uint32> mesh_num_indices;
				std::vector<uint32> mesh_start_indices(1, 0);
				char is_index_16_bit = true;
				for (XmlNodePtr mesh_node = meshes_chunk->FirstNode("mesh"); mesh_node; mesh_node = mesh_node->NextSibling("mesh"))
				{
					{
						XmlNodePtr vertices_chunk = mesh_node->FirstNode("vertices_chunk");

						bool has_position = false;
						bool has_normal = false;
						bool has_diffuse = false;
						bool has_specular = false;
						bool has_weight = false;
						std::vector<uint32> max_num_tc_components;
						bool has_tangent = false;
						bool has_binormal = false;

						uint32 num_vertices = 0;
						for (XmlNodePtr vertex_node = vertices_chunk->FirstNode("vertex"); vertex_node; vertex_node = vertex_node->NextSibling("vertex"))
						{
							++ num_vertices;

							has_position = true;
						
							XmlNodePtr normal_node = vertex_node->FirstNode("normal");
							if (normal_node)
							{
								has_normal = true;
							}

							XmlNodePtr diffuse_node = vertex_node->FirstNode("diffuse");
							if (diffuse_node)
							{
								has_diffuse = true;
							}

							XmlNodePtr specular_node = vertex_node->FirstNode("specular");
							if (specular_node)
							{
								has_specular = true;
							}

							for (XmlNodePtr weight_node = vertex_node->FirstNode("weight"); weight_node; weight_node = weight_node->NextSibling("weight"))
							{
								has_weight = true;
							}

							uint32 num_tex_coord = 0;
							for (XmlNodePtr tex_coord_node = vertex_node->FirstNode("tex_coord"); tex_coord_node; tex_coord_node = tex_coord_node->NextSibling("tex_coord"))
							{
								++ num_tex_coord;

								if (num_tex_coord >= max_num_tc_components.size())
								{
									max_num_tc_components.resize(num_tex_coord, 0);
								}

								uint32 num_components = 0;
								if (tex_coord_node->Attrib("u"))
								{
									num_components = 1;
								}
								if (tex_coord_node->Attrib("v"))
								{
									num_components = 2;
								}
								if (tex_coord_node->Attrib("w"))
								{
									num_components = 3;
								}
								max_num_tc_components[num_tex_coord - 1] = std::max(max_num_tc_components[num_tex_coord - 1], num_components);
							}

							XmlNodePtr tangent_node = vertex_node->FirstNode("tangent");
							if (tangent_node)
							{
								has_tangent = true;
							}

							XmlNodePtr binormal_node = vertex_node->FirstNode("binormal");
							if (binormal_node)
							{
								has_binormal = true;
							}
						}

						mesh_num_vertices.push_back(num_vertices);
						mesh_base_vertices.push_back(mesh_base_vertices.back() + num_vertices);

						ves.push_back(std::vector<vertex_element>());
						std::vector<vertex_element>& vertex_elements = ves.back();
						{
							vertex_element ve;

							{
								BOOST_ASSERT(has_position);

								ve.usage = VEU_Position;
								ve.usage_index = 0;
								ve.format = EF_BGR32F;
								vertex_elements.push_back(ve);
							}

							if (has_normal)
							{
								ve.usage = VEU_Normal;
								ve.usage_index = 0;
								ve.format = tbn_format;
								vertex_elements.push_back(ve);
							}

							if (has_diffuse)
							{
								ve.usage = VEU_Diffuse;
								ve.usage_index = 0;
								ve.format = EF_ABGR32F;
								vertex_elements.push_back(ve);
							}

							if (has_specular)
							{
								ve.usage = VEU_Specular;
								ve.usage_index = 0;
								ve.format = EF_ABGR32F;
								vertex_elements.push_back(ve);
							}

							if (has_weight)
							{
								ve.usage = VEU_BlendWeight;
								ve.usage_index = 0;
								ve.format = EF_ABGR32F;
								vertex_elements.push_back(ve);

								ve.usage = VEU_BlendIndex;
								ve.usage_index = 0;
								ve.format = EF_ABGR8UI;
								vertex_elements.push_back(ve);
							}

							for (uint32_t usage = 0; usage < max_num_tc_components.size(); ++ usage)
							{
								ve.usage = VEU_TextureCoord;
								ve.usage_index = static_cast<uint8_t>(usage);
								switch (max_num_tc_components[usage])
								{
								case 1:
									ve.format = EF_R32F;
									break;

								case 2:
									ve.format = EF_GR32F;
									break;

								case 3:
									ve.format = EF_BGR32F;
									break;

								default:
									ve.format = EF_ABGR32F;
									break;
								}
								vertex_elements.push_back(ve);
							}

							if (has_tangent)
							{
								ve.usage = VEU_Tangent;
								ve.usage_index = 0;
								ve.format = tbn_format;
								vertex_elements.push_back(ve);
							}

							if (has_binormal)
							{
								ve.usage = VEU_Binormal;
								ve.usage_index = 0;
								ve.format = tbn_format;
								vertex_elements.push_back(ve);
							}
						}
					}

					{
						XmlNodePtr triangles_chunk = mesh_node->FirstNode("triangles_chunk");

						uint32_t num_indices = 0;
						for (XmlNodePtr tri_node = triangles_chunk->FirstNode("triangle"); tri_node; tri_node = tri_node->NextSibling("triangle"))
						{
							num_indices += 3;
						}

						char is_index_16 = true;
						for (XmlNodePtr tri_node = triangles_chunk->FirstNode("triangle"); tri_node; tri_node = tri_node->NextSibling("triangle"))
						{
							uint32 a = static_cast<uint32>(tri_node->Attrib("a")->ValueUInt());
							uint32 b = static_cast<uint32>(tri_node->Attrib("b")->ValueUInt());
							uint32 c = static_cast<uint32>(tri_node->Attrib("c")->ValueUInt());
							if ((a > 0xFFFF) || (b > 0xFFFF) || (c > 0xFFFF))
							{
								is_index_16 = false;
							}
						}
						is_index_16_bit &= is_index_16;

						mesh_num_indices.push_back(num_indices);
						mesh_start_indices.push_back(mesh_start_indices.back() + num_indices);
					}
				}

				std::vector<vertex_element> merged_ves;
				std::vector<std::vector<uint32> > ves_mapping(ves.size());
				uint32_t mesh_index = 0;
				for (XmlNodePtr mesh_node = meshes_chunk->FirstNode("mesh"); mesh_node; mesh_node = mesh_node->NextSibling("mesh"), ++ mesh_index)
				{
					XmlNodePtr vertices_chunk = mesh_node->FirstNode("vertices_chunk");

					ves_mapping[mesh_index].resize(ves[mesh_index].size());
					for (uint32_t ve_index = 0; ve_index < ves[mesh_index].size(); ++ ve_index)
					{
						bool found = false;
						for (uint32_t mve_index = 0; mve_index < merged_ves.size(); ++ mve_index)
						{
							if (ves[mesh_index][ve_index] == merged_ves[mve_index])
							{
								ves_mapping[mesh_index][ve_index] = mve_index;
								found = true;
								break;
							}
						}
						if (!found)
						{
							ves_mapping[mesh_index][ve_index] = static_cast<uint32_t>(merged_ves.size());
							merged_ves.push_back(ves[mesh_index][ve_index]);
						}
					}
				}

				std::vector<std::vector<uint8_t> > merged_buff(merged_ves.size());
				for (size_t i = 0; i < merged_buff.size(); ++ i)
				{
					merged_buff[i].resize(mesh_base_vertices.back() * merged_ves[i].element_size(), 0);
				}

				int const index_elem_size = is_index_16_bit ? 2 : 4;

				std::vector<uint8_t> merged_indices(mesh_start_indices.back() * index_elem_size);
				std::vector<Box> bounding_boxes;
				mesh_index = 0;
				for (XmlNodePtr mesh_node = meshes_chunk->FirstNode("mesh"); mesh_node; mesh_node = mesh_node->NextSibling("mesh"), ++ mesh_index)
				{
					Vector4 min_bb, max_bb;
						
					{
						XmlNodePtr vertices_chunk = mesh_node->FirstNode("vertices_chunk");

						uint32_t index = 0;
						for (XmlNodePtr vertex_node = vertices_chunk->FirstNode("vertex"); vertex_node; vertex_node = vertex_node->NextSibling("vertex"))
						{
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_Position == ves[mesh_index][i].usage)
									{
									   Vector4 pos(vertex_node->Attrib("x")->ValueFloat(),
											vertex_node->Attrib("y")->ValueFloat(), vertex_node->Attrib("z")->ValueFloat());
										uint32 buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &pos, sizeof(pos));
										if (0 == index)
										{
											min_bb = max_bb = pos;
										}
										else
										{
											min_bb = MathLib::minimize(min_bb, pos);
											max_bb = MathLib::maximize(max_bb, pos);
										}
										break;
									}
								}
							}

							XmlNodePtr normal_node = vertex_node->FirstNode("normal");
							if (normal_node)
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_Normal == ves[mesh_index][i].usage)
									{
										Vector4 normal(normal_node->Attrib("x")->ValueFloat(),
											normal_node->Attrib("y")->ValueFloat(), normal_node->Attrib("z")->ValueFloat());
										normal = MathLib::normalize(normal) * 0.5f + 0.5f;

										uint32_t compact;
										if (EF_A2BGR10 == ves[mesh_index][i].format)
										{	
											compact = MathLib::clamp<uint32_t>(static_cast<uint32_t>(normal.x() * 1023), 0, 1023)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(normal.y() * 1023), 0, 1023) << 10)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(normal.z() * 1023), 0, 1023) << 20);
										}
										else
										{
											BOOST_ASSERT(EF_ARGB8 == ves[mesh_index][i].format);

											compact = (MathLib::clamp<uint32_t>(static_cast<uint32_t>(normal.x() * 255), 0, 255) << 16)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(normal.y() * 255), 0, 255) << 8)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(normal.z() * 255), 0, 255) << 0);
										}

										uint32 buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &compact, sizeof(compact));
										break;
									}
								}
							}

							XmlNodePtr diffuse_node = vertex_node->FirstNode("diffuse");
							if (diffuse_node)
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_Diffuse == ves[mesh_index][i].usage)
									{
										Vector4 diffuse(diffuse_node->Attrib("r")->ValueFloat(), diffuse_node->Attrib("g")->ValueFloat(),
											diffuse_node->Attrib("b")->ValueFloat(), diffuse_node->Attrib("a")->ValueFloat());
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &diffuse, sizeof(diffuse));
										break;
									}
								}
							}

							XmlNodePtr specular_node = vertex_node->FirstNode("specular");
							if (specular_node)
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_Specular == ves[mesh_index][i].usage)
									{
										Vector4 specular(specular_node->Attrib("r")->ValueFloat(), specular_node->Attrib("g")->ValueFloat(),
											specular_node->Attrib("b")->ValueFloat(), specular_node->Attrib("a")->ValueFloat());
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &specular, sizeof(specular));
										break;
									}
								}
							}

							uint32 num_blend = 0;
							for (XmlNodePtr weight_node = vertex_node->FirstNode("weight"); weight_node; weight_node = weight_node->NextSibling("weight"))
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_BlendIndex == ves[mesh_index][i].usage)
									{
										uint8_t bone_index = static_cast<uint8_t>(weight_node->Attrib("bone_index")->ValueUInt());
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size() + num_blend * sizeof(bone_index)], &bone_index, sizeof(bone_index));
										break;
									}
								}
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_BlendWeight == ves[mesh_index][i].usage)
									{
										float weight = weight_node->Attrib("weight")->ValueFloat();
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size() + num_blend * sizeof(weight)], &weight, sizeof(weight));
										break;
									}
								}
								++ num_blend;
							}
							for (uint32_t b = num_blend; b < 4; ++ b)
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_BlendIndex == ves[mesh_index][i].usage)
									{
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memset(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size() + b * sizeof(uint8_t)], 0, sizeof(uint8_t));
										break;
									}
								}
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_BlendWeight == ves[mesh_index][i].usage)
									{
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memset(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size() + b * sizeof(float)], 0, sizeof(float));
										break;
									}
								}
							}

							uint32_t usage = 0;
							for (XMLNodePtr tex_coord_node = vertex_node->FirstNode("tex_coord"); tex_coord_node; tex_coord_node = tex_coord_node->NextSibling("tex_coord"))
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if ((VEU_TextureCoord == ves[mesh_index][i].usage) && (usage == ves[mesh_index][i].usage_index))
									{
										float3 tex_coord(0, 0, 0);
										XMLAttributePtr attr = tex_coord_node->Attrib("u");
										if (attr)
										{
											tex_coord.x() = attr->ValueFloat();
										}
										attr = tex_coord_node->Attrib("v");
										if (attr)
										{
											tex_coord.y() = attr->ValueFloat();
										}
										attr = tex_coord_node->Attrib("w");
										if (attr)
										{
											tex_coord.z() = attr->ValueFloat();
										}

										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &tex_coord, merged_ves[buf_index].element_size());
										break;
									}
								}
								++ usage;
							}

							XmlNodePtr tangent_node = vertex_node->FirstNode("tangent");
							if (tangent_node)
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_Tangent == ves[mesh_index][i].usage)
									{
										float k = 1;
										XmlAttributePtr attr = tangent_node->Attrib("w");
										if (attr)
										{
											k = attr->ValueFloat();
										}

										Vector4 tangent(tangent_node->Attrib("x")->ValueFloat(),
											tangent_node->Attrib("y")->ValueFloat(), tangent_node->Attrib("z")->ValueFloat());
										tangent = MathLib::normalize(tangent) * 0.5f + 0.5f;

										uint32_t compact;
										if (EF_A2BGR10 == ves[mesh_index][i].format)
										{	
											compact = MathLib::clamp<uint32_t>(static_cast<uint32_t>(tangent.x() * 1023), 0, 1023)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(tangent.y() * 1023), 0, 1023) << 10)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(tangent.z() * 1023), 0, 1023) << 20)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>((k * 0.5f + 0.5f) * 3), 0, 3) << 30);
										}
										else
										{
											BOOST_ASSERT(EF_ARGB8 == ves[mesh_index][i].format);

											compact = (MathLib::clamp<uint32_t>(static_cast<uint32_t>(tangent.x() * 255), 0, 255) << 16)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(tangent.y() * 255), 0, 255) << 8)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(tangent.z() * 255), 0, 255) << 0)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>((k * 0.5f + 0.5f) * 255), 0, 255) << 24);
										}
										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &compact, sizeof(compact));
										break;
									}
								}
							}

							XMLNodePtr binormal_node = vertex_node->FirstNode("binormal");
							if (binormal_node)
							{
								for (size_t i = 0; i < ves[mesh_index].size(); ++ i)
								{
									if (VEU_Binormal == ves[mesh_index][i].usage)
									{
										float3 binormal(tangent_node->Attrib("x")->ValueFloat(),
											tangent_node->Attrib("y")->ValueFloat(), tangent_node->Attrib("z")->ValueFloat());
										binormal = MathLib::normalize(binormal) * 0.5f + 0.5f;

										uint32_t compact;
										if (EF_A2BGR10 == ves[mesh_index][i].format)
										{	
											compact = MathLib::clamp<uint32_t>(static_cast<uint32_t>(binormal.x() * 1023), 0, 1023)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(binormal.y() * 1023), 0, 1023) << 10)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(binormal.z() * 1023), 0, 1023) << 20);
										}
										else
										{
											BOOST_ASSERT(EF_ARGB8 == ves[mesh_index][i].format);

											compact = (MathLib::clamp<uint32_t>(static_cast<uint32_t>(binormal.x() * 255), 0, 255) << 16)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(binormal.y() * 255), 0, 255) << 8)
												| (MathLib::clamp<uint32_t>(static_cast<uint32_t>(binormal.z() * 255), 0, 255) << 0);
										}

										uint32_t buf_index = ves_mapping[mesh_index][i];
										memcpy(&merged_buff[buf_index][(mesh_base_vertices[mesh_index] + index) * merged_ves[buf_index].element_size()], &compact, sizeof(compact));
										break;
									}
								}
							}

							++ index;
						}
					}

					bounding_boxes.push_back(Box(min_bb, max_bb));

					{
						XMLNodePtr triangles_chunk = mesh_node->FirstNode("triangles_chunk");

						uint32_t index = 0;
						for (XMLNodePtr tri_node = triangles_chunk->FirstNode("triangle"); tri_node; tri_node = tri_node->NextSibling("triangle"))
						{
							if (is_index_16_bit)
							{
								uint16_t a = static_cast<uint16_t>(tri_node->Attrib("a")->ValueUInt());
								uint16_t b = static_cast<uint16_t>(tri_node->Attrib("b")->ValueUInt());
								uint16_t c = static_cast<uint16_t>(tri_node->Attrib("c")->ValueUInt());
								memcpy(&merged_indices[(mesh_start_indices[mesh_index] + index * 3 + 0) * index_elem_size], &a, sizeof(a));
								memcpy(&merged_indices[(mesh_start_indices[mesh_index] + index * 3 + 1) * index_elem_size], &b, sizeof(b));
								memcpy(&merged_indices[(mesh_start_indices[mesh_index] + index * 3 + 2) * index_elem_size], &c, sizeof(c));
							}
							else
							{
								uint32_t a = static_cast<uint32_t>(tri_node->Attrib("a")->ValueUInt());
								uint32_t b = static_cast<uint32_t>(tri_node->Attrib("b")->ValueUInt());
								uint32_t c = static_cast<uint32_t>(tri_node->Attrib("c")->ValueUInt());
								memcpy(&merged_indices[(mesh_start_indices[mesh_index] + index * 3 + 0) * index_elem_size], &a, sizeof(a));
								memcpy(&merged_indices[(mesh_start_indices[mesh_index] + index * 3 + 1) * index_elem_size], &b, sizeof(b));
								memcpy(&merged_indices[(mesh_start_indices[mesh_index] + index * 3 + 2) * index_elem_size], &c, sizeof(c));
							}

							++ index;
						}
					}
				}

				uint32_t num_merged_ves = static_cast<uint32_t>(merged_ves.size());
				NativeToLittleEndian<sizeof(num_merged_ves)>(&num_merged_ves);
				ss->write(reinterpret_cast<char*>(&num_merged_ves), sizeof(num_merged_ves));
				for (size_t i = 0; i < merged_ves.size(); ++ i)
				{
					NativeToLittleEndian<sizeof(merged_ves[i].usage)>(&merged_ves[i].usage);
					NativeToLittleEndian<sizeof(merged_ves[i].format)>(&merged_ves[i].format);
					ss->write(reinterpret_cast<char*>(&merged_ves[i]), sizeof(merged_ves[i]));
				}

				uint32_t num_vertices = mesh_base_vertices.back();
				NativeToLittleEndian<sizeof(num_vertices)>(&num_vertices);
				ss->write(reinterpret_cast<char*>(&num_vertices), sizeof(num_vertices));
				uint32_t num_indices = mesh_start_indices.back();
				NativeToLittleEndian<sizeof(num_indices)>(&num_indices);
				ss->write(reinterpret_cast<char*>(&num_indices), sizeof(num_indices));
				ss->write(&is_index_16_bit, sizeof(is_index_16_bit));

				for (size_t i = 0; i < merged_buff.size(); ++ i)
				{
					ss->write(reinterpret_cast<char*>(&merged_buff[i][0]), merged_buff[i].size() * sizeof(merged_buff[i][0]));
				}
				ss->write(reinterpret_cast<char*>(&merged_indices[0]), merged_indices.size() * sizeof(merged_indices[0]));

				mesh_index = 0;
				for (XmlNodePtr mesh_node = meshes_chunk->FirstNode("mesh"); mesh_node; mesh_node = mesh_node->NextSibling("mesh"))
				{
					WriteShortString(*ss, mesh_node->Attrib("name")->ValueString());

					int32_t mtl_id = mesh_node->Attrib("mtl_id")->ValueInt();
					NativeToLittleEndian<sizeof(mtl_id)>(&mtl_id);
					ss->write(reinterpret_cast<char*>(&mtl_id), sizeof(mtl_id));

					float3 min_bb = bounding_boxes[mesh_index].Min();
					NativeToLittleEndian<sizeof(min_bb[0])>(&min_bb[0]);
					NativeToLittleEndian<sizeof(min_bb[1])>(&min_bb[1]);
					NativeToLittleEndian<sizeof(min_bb[2])>(&min_bb[2]);
					ss->write(reinterpret_cast<char*>(&min_bb), sizeof(min_bb));
					float3 max_bb = bounding_boxes[mesh_index].Max();
					NativeToLittleEndian<sizeof(max_bb[0])>(&max_bb[0]);
					NativeToLittleEndian<sizeof(max_bb[1])>(&max_bb[1]);
					NativeToLittleEndian<sizeof(max_bb[2])>(&max_bb[2]);
					ss->write(reinterpret_cast<char*>(&max_bb), sizeof(max_bb));

					NativeToLittleEndian<sizeof(mesh_num_vertices[mesh_index])>(&mesh_num_vertices[mesh_index]);
					ss->write(reinterpret_cast<char*>(&mesh_num_vertices[mesh_index]), sizeof(mesh_num_vertices[mesh_index]));
					NativeToLittleEndian<sizeof(mesh_base_vertices[mesh_index])>(&mesh_base_vertices[mesh_index]);
					ss->write(reinterpret_cast<char*>(&mesh_base_vertices[mesh_index]), sizeof(mesh_base_vertices[mesh_index]));
					NativeToLittleEndian<sizeof(mesh_num_indices[mesh_index])>(&mesh_num_indices[mesh_index]);
					ss->write(reinterpret_cast<char*>(&mesh_num_indices[mesh_index]), sizeof(mesh_num_indices[mesh_index]));
					NativeToLittleEndian<sizeof(mesh_start_indices[mesh_index])>(&mesh_start_indices[mesh_index]);
					ss->write(reinterpret_cast<char*>(&mesh_start_indices[mesh_index]), sizeof(mesh_start_indices[mesh_index]));

					++ mesh_index;
				}
			}

			if (bones_chunk)
			{
				for (XmlNodePtr bone_node = bones_chunk->FirstNode("bone"); bone_node; bone_node = bone_node->NextSibling("bone"))
				{
					WriteShortString(*ss, bone_node->Attrib("name")->ValueString());

					int16_t joint_parent = static_cast<int16_t>(bone_node->Attrib("parent")->ValueInt());
					NativeToLittleEndian<sizeof(joint_parent)>(&joint_parent);
					ss->write(reinterpret_cast<char*>(&joint_parent), sizeof(joint_parent));

					XmlNodePtr bind_pos_node = bone_node->FirstNode("bind_pos");
					if (bind_pos_node)
					{
						Vector4 bind_pos(bind_pos_node->Attrib("x")->ValueFloat(), bind_pos_node->Attrib("y")->ValueFloat(),
							bind_pos_node->Attrib("z")->ValueFloat());

						XmlNodePtr bind_quat_node = bone_node->FirstNode("bind_quat");
						Quaternion bind_quat(bind_quat_node->Attrib("x")->ValueFloat(), bind_quat_node->Attrib("y")->ValueFloat(),
							bind_quat_node->Attrib("z")->ValueFloat(), bind_quat_node->Attrib("w")->ValueFloat());

						Quaternion bind_dual = MathLib::quat_trans_to_udq(bind_quat, bind_pos);

						NativeToLittleEndian<sizeof(bind_quat[0])>(&bind_quat[0]);
						NativeToLittleEndian<sizeof(bind_quat[1])>(&bind_quat[1]);
						NativeToLittleEndian<sizeof(bind_quat[2])>(&bind_quat[2]);
						NativeToLittleEndian<sizeof(bind_quat[3])>(&bind_quat[3]);
						ss->write(reinterpret_cast<char*>(&bind_quat), sizeof(bind_quat));
						NativeToLittleEndian<sizeof(bind_dual[0])>(&bind_dual[0]);
						NativeToLittleEndian<sizeof(bind_dual[1])>(&bind_dual[1]);
						NativeToLittleEndian<sizeof(bind_dual[2])>(&bind_dual[2]);
						NativeToLittleEndian<sizeof(bind_dual[3])>(&bind_dual[3]);
						ss->write(reinterpret_cast<char*>(&bind_dual), sizeof(bind_dual));
					}
					else
					{
						XmlNodePtr bind_real_node = bone_node->FirstNode("bind_real");
						Quaternion bind_real(bind_real_node->Attrib("x")->ValueFloat(), bind_real_node->Attrib("y")->ValueFloat(),
							bind_real_node->Attrib("z")->ValueFloat(), bind_real_node->Attrib("w")->ValueFloat());
						NativeToLittleEndian<sizeof(bind_real[0])>(&bind_real[0]);
						NativeToLittleEndian<sizeof(bind_real[1])>(&bind_real[1]);
						NativeToLittleEndian<sizeof(bind_real[2])>(&bind_real[2]);
						NativeToLittleEndian<sizeof(bind_real[3])>(&bind_real[3]);
						ss->write(reinterpret_cast<char*>(&bind_real), sizeof(bind_real));
							
						XmlNodePtr bind_dual_node = bone_node->FirstNode("bind_dual");
						Quaternion bind_dual(bind_dual_node->Attrib("x")->ValueFloat(), bind_dual_node->Attrib("y")->ValueFloat(),
							bind_dual_node->Attrib("z")->ValueFloat(), bind_dual_node->Attrib("w")->ValueFloat());
						NativeToLittleEndian<sizeof(bind_dual[0])>(&bind_dual[0]);
						NativeToLittleEndian<sizeof(bind_dual[1])>(&bind_dual[1]);
						NativeToLittleEndian<sizeof(bind_dual[2])>(&bind_dual[2]);
						NativeToLittleEndian<sizeof(bind_dual[3])>(&bind_dual[3]);
						ss->write(reinterpret_cast<char*>(&bind_dual), sizeof(bind_dual));
					}
				}
			}

			if (key_frames_chunk)
			{
				int32 start_frame = key_frames_chunk->Attrib("start_frame")->ValueInt();
				int32 end_frame = key_frames_chunk->Attrib("end_frame")->ValueInt();
				int32 frame_rate = key_frames_chunk->Attrib("frame_rate")->ValueInt();
				NativeToLittleEndian<sizeof(start_frame)>(&start_frame);
				ss->write(reinterpret_cast<char*>(&start_frame), sizeof(start_frame));
				NativeToLittleEndian<sizeof(end_frame)>(&end_frame);
				ss->write(reinterpret_cast<char*>(&end_frame), sizeof(end_frame));
				NativeToLittleEndian<sizeof(frame_rate)>(&frame_rate);
				ss->write(reinterpret_cast<char*>(&frame_rate), sizeof(frame_rate));

				SKeyFrame kfs;
				for (XmlNodePtr kf_node = key_frames_chunk->FirstNode("key_frame"); kf_node; kf_node = kf_node->NextSibling("key_frame"))
				{
					WriteShortString(*ss, kf_node->Attrib("joint")->ValueString());

					kfs.frame_id.clear();
					kfs.bind_real.clear();
					kfs.bind_dual.clear();

					int32_t frame_id = -1;
					for (XmlNodePtr key_node = kf_node->FirstNode("key"); key_node; key_node = key_node->NextSibling("key"))
					{
						XmlAttributePtr id_attr = key_node->Attrib("id");
						if (id_attr)
						{
							frame_id = id_attr->ValueInt();
						}
						else
						{
							++ frame_id;
						}
						kfs.frame_id.push_back(frame_id);

						Quaternion bind_real;
						Quaternion bind_dual;
						XMLNodePtr pos_node = key_node->FirstNode("pos");
						if (pos_node)
						{
							float3 bind_pos(pos_node->Attrib("x")->ValueFloat(), pos_node->Attrib("y")->ValueFloat(),
								pos_node->Attrib("z")->ValueFloat());

							XMLNodePtr quat_node = key_node->FirstNode("quat");
							bind_real = Quaternion(quat_node->Attrib("x")->ValueFloat(), quat_node->Attrib("y")->ValueFloat(),
								quat_node->Attrib("z")->ValueFloat(), quat_node->Attrib("w")->ValueFloat());
					
					        bind_dual = MathLib::quat_trans_to_udq(bind_real, bind_pos);
						}
						else
						{
							XMLNodePtr bind_real_node = key_node->FirstNode("bind_real");
							bind_real = Quaternion(bind_real_node->Attrib("x")->ValueFloat(), bind_real_node->Attrib("y")->ValueFloat(),
								bind_real_node->Attrib("z")->ValueFloat(), bind_real_node->Attrib("w")->ValueFloat());
							
							XMLNodePtr bind_dual_node = key_node->FirstNode("bind_dual");
							bind_dual = Quaternion(bind_dual_node->Attrib("x")->ValueFloat(), bind_dual_node->Attrib("y")->ValueFloat(),
								bind_dual_node->Attrib("z")->ValueFloat(), bind_dual_node->Attrib("w")->ValueFloat());
						}

						kfs.bind_real.push_back(bind_real);
						kfs.bind_dual.push_back(bind_dual);
					}

					// compress the key frame data
					uint32_t base = 0;
					while (base < kfs.frame_id.size() - 2)
					{
						uint32_t frame0 = kfs.frame_id[base + 0];
						uint32_t frame1 = kfs.frame_id[base + 1];
						uint32_t frame2 = kfs.frame_id[base + 2];
						std::pair<Quaternion, Quaternion> interpolate = MathLib::sclerp(kfs.bind_real[base + 0], kfs.bind_dual[base + 0],
							kfs.bind_real[base + 2], kfs.bind_dual[base + 2], static_cast<float>(frame1 - frame0) / (frame2 - frame0));

						float quat_dot = MathLib::dot(kfs.bind_real[base + 1], interpolate.first);
						Quaternion to_sign_corrected_real = interpolate.first;
						Quaternion to_sign_corrected_dual = interpolate.second;
						if (quat_dot < 0)
						{
							to_sign_corrected_real = -to_sign_corrected_real;
							to_sign_corrected_dual = -to_sign_corrected_dual;
						}

						std::pair<Quaternion, Quaternion> dif_dq = MathLib::inverse(kfs.bind_real[base + 1], kfs.bind_dual[base + 1]);
						dif_dq.second = MathLib::mul_dual(dif_dq.first, dif_dq.second, to_sign_corrected_real, to_sign_corrected_dual);
						dif_dq.first = MathLib::mul_real(dif_dq.first, to_sign_corrected_real);

						if ((abs(dif_dq.first.x()) < 1e-5f) && (abs(dif_dq.first.y()) < 1e-5f)
							&& (abs(dif_dq.first.z()) < 1e-5f) && (abs(dif_dq.first.w() - 1) < 1e-5f)
							&& (abs(dif_dq.second.x()) < 1e-5f) && (abs(dif_dq.second.y()) < 1e-5f)
							&& (abs(dif_dq.second.z()) < 1e-5f) && (abs(dif_dq.second.w()) < 1e-5f))
						{
							kfs.frame_id.erase(kfs.frame_id.begin() + base + 1);
							kfs.bind_real.erase(kfs.bind_real.begin() + base + 1);
							kfs.bind_dual.erase(kfs.bind_dual.begin() + base + 1);
						}
						else
						{
							++ base;
						}
					}

					uint32_t num_kf = static_cast<uint32_t>(kfs.frame_id.size());
					NativeToLittleEndian<sizeof(num_kf)>(&num_kf);
					ss->write(reinterpret_cast<char*>(&num_kf), sizeof(num_kf));
					for (uint32_t i = 0; i < num_kf; ++ i)
					{
						NativeToLittleEndian<sizeof(kfs.frame_id[i])>(&kfs.frame_id[i]);
						ss->write(reinterpret_cast<char*>(&kfs.frame_id[i]), sizeof(kfs.frame_id[i]));
						NativeToLittleEndian<sizeof(kfs.bind_real[i][0])>(&kfs.bind_real[i][0]);
						NativeToLittleEndian<sizeof(kfs.bind_real[i][1])>(&kfs.bind_real[i][1]);
						NativeToLittleEndian<sizeof(kfs.bind_real[i][2])>(&kfs.bind_real[i][2]);
						NativeToLittleEndian<sizeof(kfs.bind_real[i][3])>(&kfs.bind_real[i][3]);
						ss->write(reinterpret_cast<char*>(&kfs.bind_real[i]), sizeof(kfs.bind_real[i]));
						NativeToLittleEndian<sizeof(kfs.bind_dual[i][0])>(&kfs.bind_dual[i][0]);
						NativeToLittleEndian<sizeof(kfs.bind_dual[i][1])>(&kfs.bind_dual[i][1]);
						NativeToLittleEndian<sizeof(kfs.bind_dual[i][2])>(&kfs.bind_dual[i][2]);
						NativeToLittleEndian<sizeof(kfs.bind_dual[i][3])>(&kfs.bind_dual[i][3]);
						ss->write(reinterpret_cast<char*>(&kfs.bind_dual[i]), sizeof(kfs.bind_dual[i]));
					}
				}
			}

			std::ofstream ofs((path_name + jit_ext_name).c_str(), std::ios_base::binary);
			BOOST_ASSERT(ofs);
			uint32_t fourcc = MakeFourCC<'K', 'L', 'M', ' '>::value;
			NativeToLittleEndian<sizeof(fourcc)>(&fourcc);
			ofs.write(reinterpret_cast<char*>(&fourcc), sizeof(fourcc));

			uint32_t ver = MODEL_BIN_VERSION;
			NativeToLittleEndian<sizeof(ver)>(&ver);
			ofs.write(reinterpret_cast<char*>(&ver), sizeof(ver));

			uint64_t original_len = ss->str().size();
			NativeToLittleEndian<sizeof(original_len)>(&original_len);
			ofs.write(reinterpret_cast<char*>(&original_len), sizeof(original_len));

			std::ofstream::pos_type p = ofs.tellp();
			uint64_t len = 0;
			ofs.write(reinterpret_cast<char*>(&len), sizeof(len));

			LZMACodec lzma;
			len = lzma.Encode(ofs, ss->str().c_str(), ss->str().size());

			ofs.seekp(p, std::ios_base::beg);
			NativeToLittleEndian<sizeof(len)>(&len);
			ofs.write(reinterpret_cast<char*>(&len), sizeof(len));
		}
	}

	//从 OBj文件读取model
	//void RenderMesh::LoadGeometryFromOBJ(char& sFileName)
	//{
	//	
	//  // 文件讀取
	//	WCHAR wsMaterialFilename[MAX_PATH] = {0};
	//	WCHAR wstr[MAX_PATH];
	//	char str[MAX_PATH];

	//		// Find the file
	//		
	//	WideCharToMultiByte( CP_ACP, 0, wstr, -1, str, MAX_PATH, NULL, NULL );

	//		// Create temporary storage for the input data. Once the data has been loaded into
	//		// a reasonable format we can create a D3DXMesh object and load it with the mesh data.
	//	std::vector<Vector4> m_pPositions;
	//	std::vector<Vector2> m_pTexCoords;
	//	std::vector<Vector4> m_pNormals;
	//		// The first subset uses the default material
	//	IMaterial* pMaterial = new IMaterial("name");
	//		if( pMaterial == NULL )
	//			return;


	//	DWORD dwCurSubset = 0;
	//		// File input
	//	WCHAR strCommand[256] = {0};
	//	wifstream InFile( str );
	//	if( !InFile )
	//		return;

	//	for(; ; )
	//	{
	//		InFile >> strCommand;
	//		if( !InFile )
	//			break;

	//		if( 0 == wcscmp( strCommand, L"#" ) )
	//		{
	//				// Comment
	//		}
	//			else if( 0 == wcscmp( strCommand, L"v" ) )
	//		{
	//				// Vertex Position
	//			float x, y, z;
	//			InFile >> x >> y >> z;
	//			m_pPositions.push_back( Vector4( x, y, z ) );
	//		}
	//			else if( 0 == wcscmp( strCommand, L"vt" ) )
	//			{
	//				// Vertex TexCoord
	//				float u, v;
	//				InFile >> u >> v;
	//				m_pTexCoords.push_back( Vector2( u, v ) );
	//			}
	//			else if( 0 == wcscmp( strCommand, L"vn" ) )
	//			{
	//				// Vertex Normal
	//				float x, y, z;
	//				InFile >> x >> y >> z;
	//				m_pNormals.push_back( Vector4( x, y, z ) );
	//			}
	//			else if( 0 == wcscmp( strCommand, L"f" ) )
	//			{
	//				// Face
	//				UINT iPosition, iTexCoord, iNormal;
	//				SVertex vertex;

	//				for( UINT iFace = 0; iFace < 3; iFace++ )
	//				{
	//					ZeroMemory( &vertex, sizeof( SVertex ) );

	//					// OBJ format uses 1-based arrays
	//					InFile >> iPosition;
	//					vertex.position = m_pPositions[ iPosition - 1 ];

	//					if( '/' == InFile.peek() )
	//					{
	//						InFile.ignore();

	//						if( '/' != InFile.peek() )
	//						{
	//							// Optional texture coordinate
	//							InFile >> iTexCoord;
	//							vertex.texcoord = m_pTexCoords[ iTexCoord - 1 ];
	//						}

	//						if( '/' == InFile.peek() )
	//						{
	//							InFile.ignore();

	//							// Optional vertex normal
	//							InFile >> iNormal;
	//							vertex.normal = m_pNormals[ iNormal - 1 ];
	//						}
	//					}

	//					// If a duplicate vertex doesn't exist, add this vertex to the Vertices
	//					// list. Store the index in the Indices array. The Vertices and Indices
	//					// lists will eventually become the Vertex Buffer and Index Buffer for
	//					// the mesh.
	//					DWORD index = AddVertex( iPosition, &vertex );
	//					m_pIndices.push_back( index );
	//				}
	//				m_pAttributes.push_back( dwCurSubset );
	//			}
	//			else if( 0 == wcscmp( strCommand, L"mtllib" ) )
	//			{
	//				// Material library
	//				InFile >> wsMaterialFilename;
	//			}
	//			else if( 0 == wcscmp( strCommand, L"usemtl" ) )
	//			{
	//				// Material
	//				WCHAR strName[MAX_PATH] = {0};
	//				InFile >> strName;

	//				bool bFound = false;
	//				for( int iMaterial = 0; iMaterial < m_pMaterials.size(); iMaterial++ )
	//				{
	//					IMaterial pCurMaterial = m_pMaterials.at( iMaterial );
	//					if( 0 == wcscmp( pCurMaterial->strName, strName ) )
	//					{
	//						bFound = true;
	//						dwCurSubset = iMaterial;
	//						break;
	//					}
	//				}

	//				if( !bFound )
	//				{
	//					pMaterial = new CMaterial();
	//					if( pMaterial == NULL )
	//						return E_OUTOFMEMORY;

	//					dwCurSubset = m_pMaterials.size();

	//					//InitMaterial( pMaterial );
	//					wcscpy_s( pMaterial, MAX_PATH - 1, strName );

	//					m_pMaterials.push_back( pMaterial );
	//				}
	//			}
	//			else
	//			{
	//				// Unimplemented or unrecognized command
	//			}

	//			InFile.ignore( 1000, '\n' );
	//		}

	//		// Cleanup
	//		InFile.close();
	//		m_pVertexCache.resae();

	//		// If an associated material file was found, read that in as well.
	//		if( strMaterialFilename[0] )
	//		{
	//			V_RETURN( LoadMaterialsFromMTL( strMaterialFilename ) );
	//		}

	//	}

    //添加頂點索引
   // OBJ文件
	//DWORD RenderMesh::AddVertex(uint32 hash, SVertex& pVertex)
	//{
	//	bool bFoundInList = false;
	//	DWORD index = 0;

	//	// Since it's very slow to check every element in the vertex list, a hashtable stores
	//	// vertex indices according to the vertex position's index as reported by the OBJ file
	//	
	//	if( ( uint32 )m_pVertexCache.size() > hash )
	//	{
	//		SCacheEntry* pEntry = m_pVertexCache.at(hash);
	//		while( pEntry != NULL )
	//		{
	//			SVertex* pCacheVertex = m_pVertices + pEntry->index;

	//			// If this vertex is identical to the vertex already in the list, simply
	//			// point the index buffer to the existing vertex
	//			if( 0 == memcmp( pVertex, pCacheVertex, sizeof( SVertex ) ) )
	//			{
	//				bFoundInList = true;
	//				index = pEntry->index;
	//				break;
	//			}

	//			pEntry = pEntry->pNext;
	//		}
	//	}

	//	// Vertex was not found in the list. Create a new entry, both within the Vertices list
	//	// and also within the hashtable cache
	//	if( !bFoundInList )
	//	{
	//		// Add to the Vertices list
	//		index = m_pVertices.size();
	//		m_pVertices.push_back(&pVertex);
	//		// Add this to the hashtable
	//		SCacheEntry* pNewEntry = new SCacheEntry;
	//		if( pNewEntry == NULL )
	//			return;

	//		pNewEntry->index = index;
	//		pNewEntry->pNext = NULL;

	//		// Grow the cache if needed
	//		while( ( uint32 )m_pVertexCache.size() <= hash )
	//		{
	//			m_pVertexCache.push_back( NULL );
	//		}

	//		// Add to the end of the linked list
	//		CacheEntry* pCurEntry = m_VertexCache.GetAt( hash );
	//		if( pCurEntry == NULL )
	//		{
	//			// This is the head element
	//			m_VertexCache.SetAt( hash, pNewEntry );
	//		}
	//		else
	//		{
	//			// Find the tail
	//			while( pCurEntry->pNext != NULL )
	//			{
	//				pCurEntry = pCurEntry->pNext;
	//			}

	//			pCurEntry->pNext = pNewEntry;
	//		}
	//	}

	//	return index;

	//	}


	//DWORD RenderMesh::AddVertex(uint32 hash, SVertex& pVertex)
	//{
	//	FX_SetVStream()
	//}

}

namespace SE
{
	//void StaticMesh::void AddVertexStream(void const * buf, uint32 size, EVertexElementType const & ve, uint32 access_hint)
	//{
	//	m_pIR->FX_SetVStream()

	//}

	//void StaticMesh::UpdateVerteices(const void *pVertBuffer, int nVertCount, int nOffset)
	//{
	//  SVertexBuffer

	//}
}
//读取模型
namespace SE
{
 //    void LoadSEModel(ModelDesc* desc)
	//{
 //      //资源管理带填

	//	uint32 fourcc;
	//	lzma_file->read(&fourcc, sizeof(fourcc));
	//	LittleEndianToNative<sizeof(fourcc)>(&fourcc);
	//	BOOST_ASSERT((fourcc == MakeFourCC<'K', 'L', 'M', ' '>::value));

	//	uint32 ver;
	//	lzma_file->read(&ver, sizeof(ver));
	//	LittleEndianToNative<sizeof(ver)>(&ver);
	//	BOOST_ASSERT(MODEL_BIN_VERSION == ver);

	//	boost::shared_ptr<std::stringstream> ss = MakeSharedPtr<std::stringstream>();

	//	uint64_t original_len, len;
	//	lzma_file->read(&original_len, sizeof(original_len));
	//	LittleEndianToNative<sizeof(original_len)>(&original_len);
	//	lzma_file->read(&len, sizeof(len));
	//	LittleEndianToNative<sizeof(len)>(&len);

	//	LZMACodec lzma;
	//	lzma.Decode(*ss, lzma_file, len, original_len);

	//	ResIdentifierPtr decoded = MakeSharedPtr<ResIdentifier>(lzma_file->ResName(), lzma_file->Timestamp(), ss);

	//	uint32_t num_mtls;
	//	decoded->read(&num_mtls, sizeof(num_mtls));
	//	LittleEndianToNative<sizeof(num_mtls)>(&num_mtls);
	//	uint32_t num_meshes;
	//	decoded->read(&num_meshes, sizeof(num_meshes));
	//	LittleEndianToNative<sizeof(num_meshes)>(&num_meshes);
	//	uint32_t num_joints;
	//	decoded->read(&num_joints, sizeof(num_joints));
	//	LittleEndianToNative<sizeof(num_joints)>(&num_joints);
	//	uint32_t num_kfs;
	//	decoded->read(&num_kfs, sizeof(num_kfs));
	//	LittleEndianToNative<sizeof(num_kfs)>(&num_kfs);

	//	mtls.resize(num_mtls);
	//	for (uint32_t mtl_index = 0; mtl_index < num_mtls; ++ mtl_index)
	//	{
	//		RenderMaterialPtr mtl = MakeSharedPtr<RenderMaterial>();
	//		mtls[mtl_index] = mtl;

	//		decoded->read(&mtl->ambient.x(), sizeof(float));
	//		decoded->read(&mtl->ambient.y(), sizeof(float));
	//		decoded->read(&mtl->ambient.z(), sizeof(float));
	//		decoded->read(&mtl->diffuse.x(), sizeof(float));
	//		decoded->read(&mtl->diffuse.y(), sizeof(float));
	//		decoded->read(&mtl->diffuse.z(), sizeof(float));
	//		decoded->read(&mtl->specular.x(), sizeof(float));
	//		decoded->read(&mtl->specular.y(), sizeof(float));
	//		decoded->read(&mtl->specular.z(), sizeof(float));
	//		decoded->read(&mtl->emit.x(), sizeof(float));
	//		decoded->read(&mtl->emit.y(), sizeof(float));
	//		decoded->read(&mtl->emit.z(), sizeof(float));
	//		decoded->read(&mtl->opacity, sizeof(float));
	//		decoded->read(&mtl->specular_level, sizeof(float));
	//		decoded->read(&mtl->shininess, sizeof(float));

	//		LittleEndianToNative<sizeof(mtl->ambient[0])>(&mtl->ambient[0]);
	//		LittleEndianToNative<sizeof(mtl->ambient[1])>(&mtl->ambient[1]);
	//		LittleEndianToNative<sizeof(mtl->ambient[2])>(&mtl->ambient[2]);
	//		LittleEndianToNative<sizeof(mtl->diffuse[0])>(&mtl->diffuse[0]);
	//		LittleEndianToNative<sizeof(mtl->diffuse[1])>(&mtl->diffuse[1]);
	//		LittleEndianToNative<sizeof(mtl->diffuse[2])>(&mtl->diffuse[2]);
	//		LittleEndianToNative<sizeof(mtl->specular[0])>(&mtl->specular[0]);
	//		LittleEndianToNative<sizeof(mtl->specular[1])>(&mtl->specular[1]);
	//		LittleEndianToNative<sizeof(mtl->specular[2])>(&mtl->specular[2]);
	//		LittleEndianToNative<sizeof(mtl->emit[0])>(&mtl->emit[0]);
	//		LittleEndianToNative<sizeof(mtl->emit[1])>(&mtl->emit[1]);
	//		LittleEndianToNative<sizeof(mtl->emit[2])>(&mtl->emit[2]);
	//		LittleEndianToNative<sizeof(mtl->opacity)>(&mtl->opacity);
	//		LittleEndianToNative<sizeof(mtl->specular_level)>(&mtl->specular_level);
	//		LittleEndianToNative<sizeof(mtl->shininess)>(&mtl->shininess);

	//		if (Context::Instance().Config().graphics_cfg.gamma)
	//		{
	//			mtl->ambient.x() = MathLib::srgb_to_linear(mtl->ambient.x());
	//			mtl->ambient.y() = MathLib::srgb_to_linear(mtl->ambient.y());
	//			mtl->ambient.z() = MathLib::srgb_to_linear(mtl->ambient.z());
	//			mtl->diffuse.x() = MathLib::srgb_to_linear(mtl->diffuse.x());
	//			mtl->diffuse.y() = MathLib::srgb_to_linear(mtl->diffuse.y());
	//			mtl->diffuse.z() = MathLib::srgb_to_linear(mtl->diffuse.z());
	//		}

	//		uint32_t num_texs;
	//		decoded->read(&num_texs, sizeof(num_texs));
	//		LittleEndianToNative<sizeof(num_texs)>(&num_texs);

	//		for (uint32_t tex_index = 0; tex_index < num_texs; ++ tex_index)
	//		{
	//			std::string type, name;
	//			ReadShortString(decoded, type);
	//			ReadShortString(decoded, name);
	//			mtl->texture_slots.push_back(std::make_pair(type, name));
	//		}
	//	}

	//	uint32_t num_merged_ves;
	//	decoded->read(&num_merged_ves, sizeof(num_merged_ves));
	//	LittleEndianToNative<sizeof(num_merged_ves)>(&num_merged_ves);
	//	merged_ves.resize(num_merged_ves);
	//	for (size_t i = 0; i < merged_ves.size(); ++ i)
	//	{
	//		decoded->read(&merged_ves[i], sizeof(merged_ves[i]));

	//		LittleEndianToNative<sizeof(merged_ves[i].usage)>(&merged_ves[i].usage);
	//		LittleEndianToNative<sizeof(merged_ves[i].format)>(&merged_ves[i].format);
	//	}

	//	uint32_t all_num_vertices;
	//	uint32_t all_num_indices;
	//	decoded->read(&all_num_vertices, sizeof(all_num_vertices));
	//	LittleEndianToNative<sizeof(all_num_vertices)>(&all_num_vertices);
	//	decoded->read(&all_num_indices, sizeof(all_num_indices));
	//	LittleEndianToNative<sizeof(all_num_indices)>(&all_num_indices);
	//	decoded->read(&all_is_index_16_bit, sizeof(all_is_index_16_bit));

	//	int const index_elem_size = all_is_index_16_bit ? 2 : 4;

	//	merged_buff.resize(merged_ves.size());
	//	for (size_t i = 0; i < merged_buff.size(); ++ i)
	//	{
	//		merged_buff[i].resize(all_num_vertices * merged_ves[i].element_size());
	//		decoded->read(&merged_buff[i][0], merged_buff[i].size() * sizeof(merged_buff[i][0]));
	//	}
	//	merged_indices.resize(all_num_indices * index_elem_size);
	//	decoded->read(&merged_indices[0], merged_indices.size() * sizeof(merged_indices[0]));

	//	mesh_names.resize(num_meshes);
	//	mtl_ids.resize(num_meshes);
	//	bbs.resize(num_meshes);
	//	mesh_num_vertices.resize(num_meshes);
	//	mesh_base_vertices.resize(num_meshes);
	//	mesh_num_triangles.resize(num_meshes);
	//	mesh_base_triangles.resize(num_meshes);
	//	for (uint32_t mesh_index = 0; mesh_index < num_meshes; ++ mesh_index)
	//	{
	//		ReadShortString(decoded, mesh_names[mesh_index]);

	//		decoded->read(&mtl_ids[mesh_index], sizeof(mtl_ids[mesh_index]));
	//		LittleEndianToNative<sizeof(mtl_ids[mesh_index])>(&mtl_ids[mesh_index]);

	//		float3 min_bb, max_bb;
	//		decoded->read(&min_bb, sizeof(min_bb));
	//		LittleEndianToNative<sizeof(min_bb.x())>(&min_bb.x());
	//		LittleEndianToNative<sizeof(min_bb.y())>(&min_bb.y());
	//		LittleEndianToNative<sizeof(min_bb.z())>(&min_bb.z());
	//		decoded->read(&max_bb, sizeof(max_bb));
	//		LittleEndianToNative<sizeof(max_bb.x())>(&max_bb.x());
	//		LittleEndianToNative<sizeof(max_bb.y())>(&max_bb.y());
	//		LittleEndianToNative<sizeof(max_bb.z())>(&max_bb.z());
	//		bbs[mesh_index] = Box(min_bb, max_bb);

	//		decoded->read(&mesh_num_vertices[mesh_index], sizeof(mesh_num_vertices[mesh_index]));
	//		LittleEndianToNative<sizeof(mesh_num_vertices[mesh_index])>(&mesh_num_vertices[mesh_index]);
	//		decoded->read(&mesh_base_vertices[mesh_index], sizeof(mesh_base_vertices[mesh_index]));
	//		LittleEndianToNative<sizeof(mesh_base_vertices[mesh_index])>(&mesh_base_vertices[mesh_index]);
	//		decoded->read(&mesh_num_triangles[mesh_index], sizeof(mesh_num_triangles[mesh_index]));
	//		LittleEndianToNative<sizeof(mesh_num_triangles[mesh_index])>(&mesh_num_triangles[mesh_index]);
	//		decoded->read(&mesh_base_triangles[mesh_index], sizeof(mesh_base_triangles[mesh_index]));
	//		LittleEndianToNative<sizeof(mesh_base_triangles[mesh_index])>(&mesh_base_triangles[mesh_index]);
	//	}

	//	joints.resize(num_joints);
	//	for (uint32_t joint_index = 0; joint_index < num_joints; ++ joint_index)
	//	{
	//		Joint& joint = joints[joint_index];

	//		ReadShortString(decoded, joint.name);
	//		decoded->read(&joint.parent, sizeof(joint.parent));
	//		LittleEndianToNative<sizeof(joint.parent)>(&joint.parent);

	//		decoded->read(&joint.bind_real, sizeof(joint.bind_real));
	//		LittleEndianToNative<sizeof(joint.bind_real[0])>(&joint.bind_real[0]);
	//		LittleEndianToNative<sizeof(joint.bind_real[1])>(&joint.bind_real[1]);
	//		LittleEndianToNative<sizeof(joint.bind_real[2])>(&joint.bind_real[2]);
	//		LittleEndianToNative<sizeof(joint.bind_real[3])>(&joint.bind_real[3]);
	//		decoded->read(&joint.bind_dual, sizeof(joint.bind_dual));
	//		LittleEndianToNative<sizeof(joint.bind_dual[0])>(&joint.bind_dual[0]);
	//		LittleEndianToNative<sizeof(joint.bind_dual[1])>(&joint.bind_dual[1]);
	//		LittleEndianToNative<sizeof(joint.bind_dual[2])>(&joint.bind_dual[2]);
	//		LittleEndianToNative<sizeof(joint.bind_dual[3])>(&joint.bind_dual[3]);

	//		std::pair<Quaternion, Quaternion> inv = MathLib::inverse(joint.bind_real, joint.bind_dual);
	//		joint.inverse_origin_real = inv.first;
	//		joint.inverse_origin_dual = inv.second;
	//	}

	//	if (num_kfs > 0)
	//	{
	//		decoded->read(&start_frame, sizeof(start_frame));
	//		LittleEndianToNative<sizeof(start_frame)>(&start_frame);
	//		decoded->read(&end_frame, sizeof(end_frame));
	//		LittleEndianToNative<sizeof(end_frame)>(&end_frame);
	//		decoded->read(&frame_rate, sizeof(frame_rate));
	//		LittleEndianToNative<sizeof(frame_rate)>(&frame_rate);

	//		kfs = MakeSharedPtr<KeyFramesType>();
	//		for (uint32_t kf_index = 0; kf_index < num_kfs; ++ kf_index)
	//		{
	//			std::string name;
	//			ReadShortString(decoded, name);

	//			uint32_t num_kf;
	//			decoded->read(&num_kf, sizeof(num_kf));
	//			LittleEndianToNative<sizeof(num_kf)>(&num_kf);

	//			KeyFrames kf;
	//			kf.frame_id.resize(num_kf);
	//			kf.bind_real.resize(num_kf);
	//			kf.bind_dual.resize(num_kf);
	//			for (uint32_t k_index = 0; k_index < num_kf; ++ k_index)
	//			{
	//				decoded->read(&kf.frame_id[k_index], sizeof(kf.frame_id[k_index]));
	//				LittleEndianToNative<sizeof(kf.frame_id[k_index])>(&kf.frame_id[k_index]);
	//				decoded->read(&kf.bind_real[k_index], sizeof(kf.bind_real[k_index]));
	//				LittleEndianToNative<sizeof(kf.bind_real[k_index][0])>(&kf.bind_real[k_index][0]);
	//				LittleEndianToNative<sizeof(kf.bind_real[k_index][1])>(&kf.bind_real[k_index][1]);
	//				LittleEndianToNative<sizeof(kf.bind_real[k_index][2])>(&kf.bind_real[k_index][2]);
	//				LittleEndianToNative<sizeof(kf.bind_real[k_index][3])>(&kf.bind_real[k_index][3]);
	//				decoded->read(&kf.bind_dual[k_index], sizeof(kf.bind_dual[k_index]));
	//				LittleEndianToNative<sizeof(kf.bind_dual[k_index][0])>(&kf.bind_dual[k_index][0]);
	//				LittleEndianToNative<sizeof(kf.bind_dual[k_index][1])>(&kf.bind_dual[k_index][1]);
	//				LittleEndianToNative<sizeof(kf.bind_dual[k_index][2])>(&kf.bind_dual[k_index][2]);
	//				LittleEndianToNative<sizeof(kf.bind_dual[k_index][3])>(&kf.bind_dual[k_index][3]);
	//			}

	//			kfs->insert(std::make_pair(name, kf));
	//		}
	//	}
	//}

}