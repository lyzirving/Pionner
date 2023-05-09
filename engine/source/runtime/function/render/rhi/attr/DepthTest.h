#ifndef __RHI_DEPTH_TEST_H__
#define __RHI_DEPTH_TEST_H__

namespace Pionner
{
	class DepthTest
	{
	public:
		DepthTest() : m_enbale(false)
		{
		};

		~DepthTest() = default;

	public:
		bool          m_enbale;
	};
}

#endif