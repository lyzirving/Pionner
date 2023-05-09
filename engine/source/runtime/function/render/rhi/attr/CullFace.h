#ifndef __RHI_CULL_FACE_H__
#define __RHI_CULL_FACE_H__

namespace Pionner
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
		CullFace() : m_faceDir(COUNTER_CLOCK_WISE), m_mode(CULL_BACK), m_enbale(false)
		{
		};

		~CullFace() = default;

	public:
		FaceDirection m_faceDir;
		CullMode      m_mode;
		bool          m_enbale;
	};
}

#endif