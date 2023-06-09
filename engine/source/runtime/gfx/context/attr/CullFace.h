#ifndef __PIONNER_GFX_CONTEXT_ATTR_CULL_FACE_H__
#define __PIONNER_GFX_CONTEXT_ATTR_CULL_FACE_H__

namespace pio
{
	namespace gfx
	{
		enum FaceDirection
		{
			CLOCK_WISE,
			COUNTER_CLOCK_WISE
		};

		enum CullMode
		{
			CULL_BACK,
			CULL_FRONT,
			CULL_FRONT_AND_BACK
		};

		class CullFace
		{
		public:
			CullFace() {};

			static CullFace disable()
			{
				CullFace cull{};
				cull.m_enable = false;
				return cull;
			}

			static CullFace common()
			{
				CullFace cull{};
				cull.m_enable = true;
				cull.m_faceDir = COUNTER_CLOCK_WISE;
				cull.m_mode = CULL_BACK;
				return cull;
			}

			~CullFace() = default;

		public:
			FaceDirection m_faceDir{ COUNTER_CLOCK_WISE };
			CullMode      m_mode{ CULL_BACK };
			bool          m_enable{ false };
		};
	}
}

#endif