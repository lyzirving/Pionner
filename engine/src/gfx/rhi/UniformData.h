#ifndef __PIONNER_GFX_RHI_UNIFORM_DATA_H__
#define __PIONNER_GFX_RHI_UNIFORM_DATA_H__

#include "RhiDef.h"

namespace pio
{
	#define PIO_UNIFORM_DATA_OVEERIDE(T)	public:\
											virtual void read(void *reader, uint32_t offset = 0) const override\
											{\
												T *p = static_cast<T *>(reader);\
												(*p) = m_buffer.read<T>(offset);\
											}\
											virtual void write(const void *data, uint32_t offset = 0) override\
											{\
												m_buffer.writeAt(data, sizeof(T), offset);\
											}

	class UniformData
	{
	public:
		UniformData(UniformDataType type, const std::string &name = "Uniform None") 
			: m_type(type), m_name(name), m_byteUsed(Rhi::GetUniformByteSize(type))
		{
			m_buffer.allocate(0, m_byteUsed);
		}

		virtual ~UniformData() {  m_buffer.release(); }
		virtual void read(void *reader, uint32_t offset = 0) const {/*do nothing*/}
		virtual void write(const void *data, uint32_t offset = 0) {/*do nothing*/}

		UniformDataType type() const { return m_type; }
		const std::string &name() const { return m_name; }
		uint32_t byteUsed() const { return m_byteUsed; }

	protected:
		UniformDataType m_type{ UniformDataType::None };
		std::string m_name{};
		uint32_t m_byteUsed{ 0 };
		Buffer m_buffer{};				
	};

	class UniformBool : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(bool)
	public:
		UniformBool(const std::string &name) : UniformData(UniformDataType::Bool, name) {}
	};

	class UniformInt : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(int32_t)
	public:
		UniformInt(const std::string &name) : UniformData(UniformDataType::Int, name) {}
	};

	class UniformUInt : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(uint32_t)
	public:
		UniformUInt(const std::string &name) : UniformData(UniformDataType::UInt, name) {}
	};

	class UniformFloat : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(float)
	public:
		UniformFloat(const std::string &name) : UniformData(UniformDataType::Float, name) {}
	};

	class UniformVec2 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::vec2)
	public:
		UniformVec2(const std::string &name) : UniformData(UniformDataType::Vec2, name) {}
	};

	class UniformVec3 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::vec3)
	public:
		UniformVec3(const std::string &name) : UniformData(UniformDataType::Vec3, name) {}
	};

	class UniformVec4 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::vec4)
	public:
		UniformVec4(const std::string &name) : UniformData(UniformDataType::Vec4, name) {}
	};

	class UniformIVec2 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::ivec2)
	public:
		UniformIVec2(const std::string &name) : UniformData(UniformDataType::IVec2, name) {}
	};

	class UniformIVec3 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::ivec3)
	public:
		UniformIVec3(const std::string &name) : UniformData(UniformDataType::IVec3, name) {}
	};

	class UniformIVec4 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::ivec4)
	public:
		UniformIVec4(const std::string &name) : UniformData(UniformDataType::IVec4, name) {}
	};

	class UniformMat2 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::mat2)
	public:
		UniformMat2(const std::string &name) : UniformData(UniformDataType::Mat2, name) {}
	};

	class UniformMat3 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::mat3)
	public:
		UniformMat3(const std::string &name) : UniformData(UniformDataType::Mat3, name) {}
	};

	class UniformMat4 : public UniformData
	{
		PIO_UNIFORM_DATA_OVEERIDE(glm::mat4)
	public:
		UniformMat4(const std::string &name) : UniformData(UniformDataType::Mat4, name) {}
	};
}

#endif