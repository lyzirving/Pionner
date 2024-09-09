#ifndef __PIONNER_UI_VIEW_MGR_H__
#define __PIONNER_UI_VIEW_MGR_H__

#include "Entry.h"

namespace pio
{
	class RenderContext;
	class RenderTarget;

	class ViewMgr
	{
	public:
		ViewMgr();
		~ViewMgr() = default;

		void onUpdate(Ref<RenderContext>& context, Ref<RenderTarget>& target);
	};
}

#endif