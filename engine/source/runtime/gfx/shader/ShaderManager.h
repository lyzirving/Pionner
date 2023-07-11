#ifndef __PIONNER_GFX_SHADER_MANAGER_H__
#define __PIONNER_GFX_SHADER_MANAGER_H__

#include <memory>
#include <mutex>
#include "render/shader/ShaderDef.h"
#include "render/shader/Shader.h"

namespace pio
{
	namespace gfx
	{
		class ShaderManager
		{
		public:
			static ShaderManager *self();

			std::shared_ptr<Shader> get(ShaderType type);
			void shutdown();

		private:
			static ShaderManager *g_instance;
			static std::mutex     g_objMutex;

			ShaderManager();
			~ShaderManager();

		private:
			std::shared_ptr<Shader> m_shaders[SHADER_TYPE_CNT];
		};
	}
}

#endif