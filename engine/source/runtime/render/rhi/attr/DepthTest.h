#ifndef __RHI_DEPTH_TEST_H__
#define __RHI_DEPTH_TEST_H__

namespace pio
{
	class DepthTest
	{
	public:
		DepthTest() : m_enbale(false)
		{
		};

		~DepthTest() = default;

		static DepthTest DepthTest::common()
		{
			DepthTest test;
			test.m_enbale = true;
			return test;
		}

		static DepthTest DepthTest::disnable()
		{
			DepthTest test;
			test.m_enbale = false;
			return test;
		}

	public:
		bool          m_enbale;
	};
}

#endif