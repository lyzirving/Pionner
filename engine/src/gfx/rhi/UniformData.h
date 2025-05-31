#pragma once

#include "gfx/GfxDef.h"

namespace pio
{
	#define PIO_UNIFORM_DATA_OVEERIDE(T)	public:\
											virtual void read(void *reader, uint32_t offset = 0) const override\
											{\
												T *p = static_cast<T *>(reader);\
												(*p) = m_Buffer.read<T>(offset);\
											}\
											virtual void Write(const void *data, uint32_t offset = 0) override\
											{\
												m_Buffer.WriteAt(data, sizeof(T), offset);\
											}

	class UniformData
	{
	public:
		UniformData(UniformType type, const std::string& name = "Uniform None");

		UniformData(const UniformData& rhs);
		UniformData(UniformData&& rhs) noexcept;

		UniformData& operator=(const UniformData& rhs);
		UniformData& operator=(UniformData&& rhs) noexcept;

		virtual ~UniformData() {  m_Buffer.release(); }
		virtual void read(void *reader, uint32_t offset = 0) const {/*do nothing*/}
		virtual void Write(const void *data, uint32_t offset = 0) {/*do nothing*/}

		UniformType Type() const { return m_Type; }
		const std::string &Name() const { return m_Name; }
		uint32_t byteUsed() const { return m_ByteUsed; }

	public:
		template<typename T>
		static Ref<UniformData> Create(const std::string& name) 
		{ 
			#ifdef LOCAL_TAG
			#undef LOCAL_TAG
			#endif
			#define LOCAL_TAG "UniformData"
			LOGE("err! type has not been implemented");
			std::abort();
			return Ref<UniformData>(); 
		}

	protected:
		UniformType m_Type{ UniformType::Bool };
		std::string m_Name{};
		uint32_t m_ByteUsed{ 0 };
		Buffer m_Buffer{};				
	};

	class UniformBool : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(bool)
	public:
		UniformBool(const std::string &name) : UniformData(UniformType::Bool, name) {}
	};

	class UniformInt : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(int32_t)
	public:
		UniformInt(const std::string &name) : UniformData(UniformType::Int, name) {}
	};

	class UniformUInt : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(uint32_t)
	public:
		UniformUInt(const std::string &name) : UniformData(UniformType::UInt, name) {}
	};

	class UniformFloat : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(float)
	public:
		UniformFloat(const std::string &name) : UniformData(UniformType::Float, name) {}
	};

	class UniformVec2 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::vec2)
	public:
		UniformVec2(const std::string &name) : UniformData(UniformType::Vec2, name) {}
	};

	class UniformVec3 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::vec3)
	public:
		UniformVec3(const std::string &name) : UniformData(UniformType::Vec3, name) {}
	};

	class UniformVec4 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::vec4)
	public:
		UniformVec4(const std::string &name) : UniformData(UniformType::Vec4, name) {}
	};

	class UniformIVec2 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::ivec2)
	public:
		UniformIVec2(const std::string &name) : UniformData(UniformType::IVec2, name) {}
	};

	class UniformIVec3 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::ivec3)
	public:
		UniformIVec3(const std::string &name) : UniformData(UniformType::IVec3, name) {}
	};

	class UniformIVec4 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::ivec4)
	public:
		UniformIVec4(const std::string &name) : UniformData(UniformType::IVec4, name) {}
	};

	class UniformMat2 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::mat2)
	public:
		UniformMat2(const std::string &name) : UniformData(UniformType::Mat2, name) {}
	};

	class UniformMat3 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::mat3)
	public:
		UniformMat3(const std::string &name) : UniformData(UniformType::Mat3, name) {}
	};

	class UniformMat4 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::mat4)
	public:
		UniformMat4(const std::string &name) : UniformData(UniformType::Mat4, name) {}
	};

	template<>
	Ref<UniformData> UniformData::Create<bool>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<int32_t>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<uint32_t>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<float>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::vec2>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::vec3>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::vec4>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::ivec2>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::ivec3>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::ivec4>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::mat2>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::mat3>(const std::string& name);

	template<>
	Ref<UniformData> UniformData::Create<glm::mat4>(const std::string& name);
}