#include "Path.h"
#include "Path.h"
#include "Path.h"
#include "Path.h"

#include "base/utils/LogSystem.h"

#ifdef PIO_WINDOWS
#include <direct.h>
#endif

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Path"

namespace pio
{
	static std::string k_RootDirectory{};
	static std::string k_ContentRoot{};
	static std::string k_EngineDir{};
	static std::string k_MeshRoot{};
	static std::string k_MatRoot{};
	static std::string k_ImageRoot{};
	static std::string k_ShaderRoot{};

	const char* Path::JPG_SUFFIX = "jpg";
	const char* Path::PNG_SUFFIX = "png";
	const char* Path::MAT_SUFFIX = "material";
	const char* Path::MESH_SUFFIX = "mesh";
	const char* Path::SKELETON_SUFFIX = "skeleton";
	const char* Path::ANIM_SUFFIX = "anim";
	const char* Path::SHADER_SUFFIX = "glsl";
	const char* Path::ASSET_SUFFIX = "asset";
	const char* Path::CONTENT = "content";
	const char* Path::FILE_SEPARATOR =
	#ifdef PIO_WINDOWS
		"\\";
    #else
		"/";
	#endif

	PIO_SINGLETON_IMPL(Path)

	void Path::Init(const InitParam& param)
	{
		k_RootDirectory = CurrentRunningPath();
		k_ContentRoot = std::string(k_RootDirectory)
			.append(FILE_SEPARATOR)
			.append(CONTENT);
		k_EngineDir = std::string(k_ContentRoot)
			.append(FILE_SEPARATOR)
			.append("engine");
		k_MeshRoot = std::string(k_EngineDir)
			.append(FILE_SEPARATOR)
			.append("mesh");
		k_MatRoot = std::string(k_EngineDir)
			.append(FILE_SEPARATOR)
			.append("material");
		k_ImageRoot = std::string(k_EngineDir)
			.append(FILE_SEPARATOR)
			.append("image");
		k_ShaderRoot = std::string(k_EngineDir)
			.append(FILE_SEPARATOR)
			.append("shader");

		LOGD("current root dir[%s], content[%s]",
			 k_RootDirectory.c_str(), k_ContentRoot.c_str());
	}

	void Path::Shutdown()
	{
	}

	std::string Path::CurrentRunningPath()
	{
	#ifdef PIO_WINDOWS
		char buff[1024];
		_getcwd(buff, 1024);
		std::string ret(buff);
		return ret;
	#endif
		return std::string();
	}

	const std::string& Path::RootDirectory()
	{
		return k_RootDirectory;
	}

	const std::string& Path::ContentRoot()
	{
		return k_ContentRoot;
	}

	const std::string& Path::MeshRoot()
	{
		return k_MeshRoot;
	}

	const std::string& Path::MaterialRoot()
	{
		return k_MatRoot;
	}

	const std::string& Path::ImageRoot()
	{
		return k_ImageRoot;
	}

	const std::string& Path::ShaderRoot()
	{
		return k_ShaderRoot;
	}

	std::string Path::MaterialKey(const char* name)
	{
		return std::string(MaterialRoot())
			.append(FILE_SEPARATOR)
			.append(name)
			.append(".")
			.append(ASSET_SUFFIX);
	}

	std::string Path::ImageKey(const char* name)
	{
		return std::string(ImageRoot())
			.append(FILE_SEPARATOR)
			.append(name)
			.append(".")
			.append(ASSET_SUFFIX);
	}

	std::string Path::MeshKey(const char* name)
	{
		return std::string(MeshRoot())
			.append(FILE_SEPARATOR)
			.append(name)
			.append(".")
			.append(ASSET_SUFFIX);
	}

	std::string Path::FindPathName(const std::string& path)
	{
		auto pos = path.find_last_of(FILE_SEPARATOR);
		if(pos != std::string::npos)
		{
			return path.substr(pos + 1);
		}
		return path;
	}

	std::string Path::FindSuffix(const std::string& path)
	{
		auto pos = path.find_last_of(".");
		if(pos != std::string::npos)
		{
			return path.substr(pos + 1);
		}
		return std::string();
	}

	std::string pio::Path::PathWithoutSuffix(const std::string& path)
	{
		auto pos = path.find_last_of(".");
		if(pos != std::string::npos)
		{
			return path.substr(0, pos);
		}
		return path;
	}
}