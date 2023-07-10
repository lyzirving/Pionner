#ifndef __RENDER_SHADER_MGR_H__
#define __RENDER_SHADER_MGR_H__

#include <memory>
#include <mutex>
#include "render/shader/ShaderDef.h"
#include "render/shader/Shader.h"

namespace pio
{
	class ShaderMgr
	{
		friend class RenderSystem;
	public:
		ShaderMgr();
		~ShaderMgr();

		std::shared_ptr<Shader> get(ShaderType type);
		void shutdown();

	private:
		std::shared_ptr<Shader> m_shaders[SHADER_TYPE_CNT];
	};
}

#endif