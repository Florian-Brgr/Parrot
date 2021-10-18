#include "Ptpch.hpp"
#include "GraphicsAPI.hpp"

#include "OpenGl/IndexBuffer_OPENGL.hpp"
#include "OpenGl/VertexBuffer_OPENGL.hpp"
#include "OpenGl/VertexArray_OPENGL.hpp"
#include "OpenGl/Texture_OPENGL.hpp"
#include "OpenGl/Shader_OPENGL.hpp"

namespace Parrot
{
	#ifdef PT_OPENGL
		IndexBufferAPI* CreateIndexBufferAPI(const uint32_t* indices, uint32_t count, bool isStatic)
		{
			return new IndexBuffer_OPENGL(indices, count, isStatic);
		}
		VertexBufferAPI* CreateVertexBufferAPI(const void* vertices, uint32_t size, bool isStatic)
		{
			return new VertexBuffer_OPENGL(vertices, size, isStatic);
		}
		VertexArrayAPI* CreateVertexArrayAPI(const VertexBufferAPI* vb, const VertexBufferLayout& layout)
		{
			return new VertexArray_OPENGL(vb, layout);
		}
		TextureAPI* CreateTextureAPI(const Utils::Filepath& filepath, const TextureAPI::Config& config)
		{
			return new Texture_OPENGL(filepath, config);
		}
		ShaderAPI* CreateShaderAPI(const Utils::Filepath& filepath)
		{
			return new Shader_OPENGL(filepath);
		}
	#endif // PT_OPENGL
}