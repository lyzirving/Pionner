#ifndef __GL_TEXTURE_H__
#define __GL_TEXTURE_H__

#include <string>

#include "render/resource/buffer/GfxBuffer.h"

namespace pio
{
	class GLTexture : public GfxBuffer
	{
		friend class GfxBuffer;
	public:
		GLTexture(const std::shared_ptr<RenderResourceMgr> &mgr);
		GLTexture(bool holder, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLTexture();

		inline void setPath(const char *path) { m_path = path; }
		inline std::string getPath() const { return m_path; }

		virtual void upload() override;

		virtual void bindTarget(uint32_t target = 0) override;
		virtual void unbind() override;
		virtual void deleteResource() override;
		virtual void loadRawData() override;

	private:
		std::string m_path;
		uint8_t *m_data;
		int m_width, m_height;
		int m_channel;
	};

	template<>
	bool GfxBuffer::is<GLTexture>() const;

	template<>
	void GfxBuffer::insertData<std::string>(std::string &data);
}

#endif