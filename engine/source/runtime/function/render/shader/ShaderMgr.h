#ifndef __RENDER_SHADER_MGR_H__
#define __RENDER_SHADER_MGR_H__

#include <memory>
#include <mutex>
#include "function/render/shader/ShaderDef.h"

namespace Pionner
{
	class Rhi;
	class Shader;

	class ShaderMgr
	{
	public:
		static ShaderMgr *instance();

		std::shared_ptr<Shader> get(ShaderType type, const std::shared_ptr<Rhi> &rhi);
		void destroy();

	private:
		ShaderMgr();
		~ShaderMgr();

	private:
		static ShaderMgr *g_instance;
		static std::mutex g_mutex;

		std::shared_ptr<Shader> m_shaders[SHADER_TYPE_CNT];
	};
}

#endif