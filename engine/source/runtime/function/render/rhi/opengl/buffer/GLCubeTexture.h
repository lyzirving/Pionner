#ifndef __GL_CUBE_TEXTURE_H__
#define __GL_CUBE_TEXTURE_H__

#include <string>

#include "function/render/resource/buffer/GfxBuffer.h"

namespace Pionner
{
	class GLCubeTexture : public GfxBuffer
	{
		friend class GfxBuffer;
	public:
		GLCubeTexture(const std::shared_ptr<RenderResourceMgr> &mgr);
		GLCubeTexture(bool holder, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLCubeTexture();

		virtual void upload() override;

		virtual void bindTarget(uint32_t target = 0) override;
		virtual void unbind() override;
		virtual void deleteResource() override;
		virtual void loadRawData() override;

	private:
		struct CubeTexInfo
		{
			std::string path;
			uint8_t *data;
			int width, height;
			int channel;

			CubeTexInfo() : path(), data(nullptr), width(0), height(0), channel(0)
			{
			}
		};

	private:
		CubeTexInfo m_info[6];
	};

	template<>
	bool GfxBuffer::is<GLCubeTexture>() const;

	template<>
	void GfxBuffer::insertData<std::string>(std::vector<std::string> &data);
}

#endif