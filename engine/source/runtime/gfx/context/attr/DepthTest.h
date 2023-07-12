#ifndef __PIONNER_GFX_CONTEXT_ATTR_DEPTH_TEST_H__
#define __PIONNER_GFX_CONTEXT_ATTR_DEPTH_TEST_H__

namespace pio
{
	namespace gfx
	{
		class DepthTest
		{
		public:
			DepthTest()
			{
			};

			~DepthTest() = default;

			static DepthTest common()
			{
				DepthTest test;
				test.m_enable = true;
				return test;
			}

			static DepthTest disnable()
			{
				DepthTest test;
				test.m_enable = false;
				return test;
			}

		public:
			bool m_enable{false};
		};
	}
}

#endif