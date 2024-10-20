#include "FileUtil.h"

namespace pio
{
	const char* FileUtil::ASSET_ROOT = "assets";
	const char* FileUtil::SEPARATOR  = "/";

	PIO_SINGLETON_IMPL(FileUtil)

	FileUtil::FileUtil()
	{
	}

	FileUtil::~FileUtil() = default;

	void FileUtil::Init()
	{
	}

	void FileUtil::Shutdown()
	{
	}
}