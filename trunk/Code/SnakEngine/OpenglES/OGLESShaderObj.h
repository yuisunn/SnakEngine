//
//#ifndef _OGLESShaderObj_H_
//#define _OGLESShaderObj_H_
//
//#include <boost/function.hpp>
//
//#include <SE3DEngine/Comm/Def.h>
//
//
//namespace SE
//{
//	class OGLESShaderObj : public ShaderObject
//	{
//	public:
//		OGLES2ShaderObject();
//		~OGLES2ShaderObject();
//
//		std::string GenShaderText(RenderEffect const & effect);
//
//		void SetShader(RenderEffect const & effect, std::vector<uint32_t> const & shader_desc_ids,
//			std::vector<ShaderObjectPtr> const & shared_so);
//		ShaderObjectPtr Clone(RenderEffect const & effect);
//
//		void Bind();
//		void Unbind();
//
//		GLint GetAttribLocation(VertexElementUsage usage, uint8_t usage_index);
//
//		GLuint GLSLProgram() const
//		{
//			return glsl_program_;
//		}
//
//	private:
//		struct parameter_bind_t
//		{
//			std::string combined_sampler_name;
//			RenderEffectParameterPtr param;
//			int location;
//			int shader_type;
//			int tex_sampler_bind_index;
//			boost::function<void()> func;
//		};
//		typedef std::vector<parameter_bind_t> parameter_binds_t;
//
//		std::string ConvertToELSL(std::string const & glsl, ShaderType type);
//		parameter_bind_t GetBindFunc(GLint location, RenderEffectParameterPtr const & param);
//		void CompileToNative(std::string const & shader_text, int type, std::string const & func_name);
//		void AttachGLSL(uint32_t type);
//		void LinkGLSL();
//
//	private:
//		GLuint glsl_program_;
//		boost::shared_ptr<boost::array<boost::shared_ptr<std::string>, ST_NumShaderTypes> > glsl_srcs_;
//		boost::shared_ptr<boost::array<boost::shared_ptr<std::vector<std::string> >, ST_NumShaderTypes> > pnames_;
//		boost::shared_ptr<boost::array<boost::shared_ptr<std::vector<std::string> >, ST_NumShaderTypes> > glsl_res_names_;
//		boost::shared_ptr<std::vector<VertexElementUsage> > vs_usages_;
//		boost::shared_ptr<std::vector<uint8_t> > vs_usage_indices_;
//		boost::shared_ptr<std::vector<std::string> > glsl_vs_attrib_names_;
//
//		parameter_binds_t param_binds_;
//
//		std::vector<std::pair<TexturePtr, SamplerStateObjectPtr> > samplers_;
//
//		std::vector<std::pair<std::string, std::pair<RenderEffectParameterPtr, RenderEffectParameterPtr> > > tex_sampler_binds_;
//
//		std::map<std::pair<VertexElementUsage, uint8_t>, GLint> attrib_locs_;
//	};
//
//	typedef boost::shared_ptr<OGLES2ShaderObject> OGLES2ShaderObjectPtr;
//}
//
//#endif		