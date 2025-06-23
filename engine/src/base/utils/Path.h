#pragma once

#include "base/core/CoreDef.h"

namespace pio
{
	class Path
	{
		PIO_SINGLETON_DECLARE(Path)
	public:
		static std::string CurrentRunningPath();
		static const std::string& RootDirectory();
		static const std::string& ContentRoot();

		//Engine's default mesh asset directory
		static const std::string& MeshRoot();
		//Engine's default material directory
		static const std::string& MaterialRoot();
		//Engine's default image directory
		static const std::string& ImageRoot();
		//Engine's default shader directory
		static const std::string& ShaderRoot();

		//Engine's default material identifier
		static std::string MaterialKey(const char* name);
		//Engine's default image identifier
		static std::string ImageKey(const char* name);
		//Engine's default mesh identifier
		static std::string MeshKey(const char* name);

		static std::string FindPathName(const std::string& path);
		static std::string FindSuffix(const std::string& path);
		static std::string PathWithoutSuffix(const std::string& path);

	public:
		static const char* CONTENT;
		static const char* FILE_SEPARATOR;
		static const char* JPG_SUFFIX;
		static const char* JPEG_SUFFIX;
		static const char* PNG_SUFFIX;
		static const char* HDR_SUFFIX;
		static const char* MAT_SUFFIX;
		static const char* MESH_SUFFIX;
		static const char* SKELETON_SUFFIX;
		static const char* ANIM_SUFFIX;
		static const char* SHADER_SUFFIX;
		static const char* ASSET_SUFFIX;
	};
}