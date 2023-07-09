#ifndef __PIONNER_SCENEGRAPH_VIEW_CAMERA_MAIN_H__
#define __PIONNER_SCENEGRAPH_VIEW_CAMERA_MAIN_H__

#include "Camera.h"

namespace pio
{
	namespace sgf
	{
		class MainCamera : public Camera
		{
		public:
			MainCamera();
			virtual ~MainCamera();

			virtual void update(const std::vector<std::shared_ptr<Layer>> &layers, RenderInfo &info) override;
			virtual void release() override;
		};
	}
}

#endif	