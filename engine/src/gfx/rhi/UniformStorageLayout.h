#pragma once

#include "gfx/GfxDef.h"

namespace pio
{	
	const char* GetBindingName(UBBinding binding);

	/*
	* UniformStorageLayout records the data layout of a specific data type: Bool, Float, Int, Vec2, Vec3, Vec4,
	*                                                              IVec2, IVec3, IVec4, UVec2, UVec3, UVec4,
	*/
	class UniformStorageLayout : public Object
	{
		RTTR_ENABLE(Object)		
	public:
		UniformStorageLayout();
		UniformStorageLayout(UniformType type);
		UniformStorageLayout(UniformType type, const std::string &name);
		virtual ~UniformStorageLayout() = default;
		virtual void FillMetaData();
		virtual void CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart);
		virtual uint32_t GetArrayNum() { return 0; }
		virtual std::string ToString() const override;

		uint32_t GetBaseAlign() const { return m_BaseAlign; }
		uint32_t GetAlignOffset() const { return m_AlignOffset; }
		uint32_t GetByteUsed() const { return m_ByteUsed; }
		uint32_t GetOccupied() const { return GetAlignOffset() + GetByteUsed(); }
		UniformType GetType() const { return m_Type; }
		const std::string &GetName() const { return m_Name; }

		void SetAlignOffset(uint32_t offset) { m_AlignOffset = offset; }
		void SetByteUsed(uint32_t used) { m_ByteUsed = used; }

	public:
		static bool IsPrimitive(UniformType type);

	protected:
		static uint32_t CalcAlignOffset(uint32_t occupied, uint32_t baseAlign);

	protected:
		std::string m_Name{};
		UniformType m_Type{ UniformType::Int };

		/*
		* Base align is the space one variable occupies in block, including padding,
		* and it's restricted by std140.
		*/
		uint32_t m_BaseAlign{ 0 };
		uint32_t m_AlignOffset{ 0 };
		uint32_t m_ByteUsed{ 0 };

	private:
		friend class UniformField;
		friend class UniformMatLayout;
		friend class UniformArrayLayout;
	};

	/*
	* UniformStorageLayout with data type: PrimitiveArray, // int[], float[], bool[]
	*                                      Vec2Array, Vec3Array, Vec4Array
	*/
	class UniformArrayLayout : public UniformStorageLayout
	{
		RTTR_ENABLE(UniformStorageLayout)
	public:
		UniformArrayLayout(UniformType type, uint32_t arrayNum, const std::string& name);
		virtual ~UniformArrayLayout();
		virtual void FillMetaData();
		virtual void CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual uint32_t GetArrayNum() override { return m_ArrayNum; }
		virtual std::string ToString() const override;

		UniformStorageLayout& operator[](uint32_t i) { return m_Array[i]; }

	private:
		static UniformType GetArrayDataType(UniformType type);

	protected:
		uint32_t m_ArrayNum{ 0 };
		uint32_t m_EndPadding{ 0 };
		std::vector<UniformStorageLayout> m_Array{};
	};

	/*
	* UniformStorageLayout with data type: Mat2, Mat3, Mat4
	*/
	class UniformMatLayout : public UniformStorageLayout
	{
		RTTR_ENABLE(UniformStorageLayout)
	public:
		UniformMatLayout();
		UniformMatLayout(UniformType type, uint32_t columnNum, const std::string &name);
		virtual ~UniformMatLayout();
		virtual void FillMetaData();
		virtual void CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual std::string ToString() const override;

	private:
		static UniformType GetColumnDataType(UniformType type);
		static uint32_t GetMatColumnNum(UniformType type);

	private:
		uint32_t m_ColumnNum{ 0 };
		uint32_t m_EndPadding{ 0 };
		std::vector<UniformStorageLayout> m_Columns{};

	private:
		friend class UniformField;
		friend class UniformMatArrayLayout;
	};

	/*
	* UniformStorageLayout with data type: Mat2Array, Mat3Array, Mat4Array
	*/
	class UniformMatArrayLayout : public UniformStorageLayout
	{
		RTTR_ENABLE(UniformStorageLayout)
	public:
		UniformMatArrayLayout(UniformType type, uint32_t arrayNum, const std::string &name);
		virtual ~UniformMatArrayLayout();
		virtual void FillMetaData() override;
		virtual void CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual uint32_t GetArrayNum() { return m_ArrayNum; }
		virtual std::string ToString() const override;

		UniformMatLayout &operator[](uint32_t i) { return m_MatArray[i]; }

	private:
		static UniformType GetArrayDataType(UniformType type);

	protected:
		uint32_t m_ArrayNum{ 0 };
		uint32_t m_EndPadding{ 0 };
		std::vector<UniformMatLayout> m_MatArray{};
	};

	class UniformStructLayout : public UniformStorageLayout
	{
		RTTR_ENABLE(UniformStorageLayout)
	public:
		UniformStructLayout();
		UniformStructLayout(const std::string &name);
		virtual ~UniformStructLayout();
		virtual void FillMetaData() override;
		virtual void CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual std::string ToString() const override;

		void Put(uint32_t index, const Ref<UniformStorageLayout> &data) { m_Data[index] = data; }
		Ref<UniformStorageLayout> Get(uint32_t index) { return m_Data[index]; }
		std::map<uint32_t, Ref<UniformStorageLayout>>::iterator Begin() { return m_Data.begin(); }
		std::map<uint32_t, Ref<UniformStorageLayout>>::iterator End() { return m_Data.end(); }

	private:
		uint32_t m_StartPadding{ 0 }, m_EndPadding{ 0 };
		std::map<uint32_t, Ref<UniformStorageLayout>, std::less<uint32_t>> m_Data{};
	};

	/*
	* UniformStorageLayout with data type: StructArray
	*/
	class UniformStructArrayLayout : public UniformStorageLayout
	{
		RTTR_ENABLE(UniformStorageLayout)
	public:
		UniformStructArrayLayout(const std::string &name);
		virtual ~UniformStructArrayLayout();
		virtual void FillMetaData();
		virtual void CalculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual std::string ToString() const override;

		void PushBack(const UniformStructLayout &data) { m_StructArray.push_back(data); m_BaseAlign = 0; }
		void PushBack(UniformStructLayout &&data) { m_StructArray.push_back(std::forward<UniformStructLayout>(data)); m_BaseAlign = 0; }

		UniformStructLayout &operator[](uint32_t i) { return m_StructArray[i]; }
		const UniformStructLayout& operator[](uint32_t i) const { return m_StructArray[i]; }

	protected:
		uint32_t m_EndPadding{ 0 };
		std::vector<UniformStructLayout> m_StructArray{};
	};
}