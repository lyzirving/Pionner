#ifndef __PIONNER_GFX_RHI_UNIFORM_PACK_H__
#define __PIONNER_GFX_RHI_UNIFORM_PACK_H__

#include "RhiDef.h"

namespace pio
{
	enum UBBinding : uint8_t
	{
		UBBinding_Camera = 0, UBBinding_Num
	};

	/*
	* UniformPack records the data layout of a specific data type: Bool, Float, Int, Vec2, Vec3, Vec4,
	*                                                              IVec2, IVec3, IVec4, UVec2, UVec3, UVec4,
	*/
	class UniformPack
	{
	public:
		UniformPack();
		UniformPack(UniformDataType type);
		UniformPack(UniformDataType type, const std::string &name);
		virtual ~UniformPack() = default;
		virtual void fillMetaData();
		virtual void calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart);
		virtual uint32_t getArrayNum() { return 0; }
		virtual std::string toString();

		uint32_t getBaseAlign() const { return m_baseAlign; }
		uint32_t getAlignOffset() const { return m_alignOffset; }
		uint32_t getByteUsed() const { return m_byteUsed; }
		uint32_t getOccupied() const { return getAlignOffset() + getByteUsed(); }
		UniformDataType getType() const { return m_type; }
		const std::string &getName() const { return m_name; }

		void setAlignOffset(uint32_t offset) { m_alignOffset = offset; }
		void setByteUsed(uint32_t used) { m_byteUsed = used; }

		template<class T>
		T *as() { if (is<T>()) { return static_cast<T *>(this); } else { return nullptr; } }

	public:
		static bool IsPrimitive(UniformDataType type);

	protected:
		static uint32_t CalcAlignOffset(uint32_t occupied, uint32_t baseAlign);

		template<class T>
		bool is() { return false; }

	protected:
		std::string m_name{};
		UniformDataType m_type{ UniformDataType::Int };

		/*
		* Base align is the space one variable occupies in block, including padding,
		* and it's restricted by std140.
		*/
		uint32_t m_baseAlign{ 0 };
		uint32_t m_alignOffset{ 0 };
		uint32_t m_byteUsed{ 0 };

	private:
		friend class UniformField;
		friend class UnimPackMat;
		friend class UnimPackArray;
	};

	/*
	* UniformPack with data type: Mat2, Mat3, Mat4
	*/
	class UnimPackMat : public UniformPack
	{
	public:
		UnimPackMat();
		UnimPackMat(UniformDataType type, uint32_t columnNum, const std::string &name);
		virtual ~UnimPackMat();
		virtual void fillMetaData();
		virtual void calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual std::string toString() override;

	private:
		static UniformDataType GetColumnDataType(UniformDataType type);
		static uint32_t GetMatColumnNum(UniformDataType type);

	private:
		uint32_t m_columnNum{ 0 };
		uint32_t m_endPadding{ 0 };
		std::vector<UniformPack> m_columns{};

	private:
		friend class UniformField;
		friend class UnimPackMatArray;
	};

	/*
	* UniformPack with data type: PrimitiveArray, // int[], float[], bool[]
	*                             Vec2Array, Vec3Array, Vec4Array
	*/
	class UnimPackArray : public UniformPack
	{
	public:
		UnimPackArray(UniformDataType type, uint32_t arrayNum, const std::string &name);
		virtual ~UnimPackArray();
		virtual void fillMetaData();
		virtual void calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual uint32_t getArrayNum() override { return m_arrayNum; }
		virtual std::string toString() override;

	private:
		static UniformDataType GetArrayDataType(UniformDataType type);

	protected:
		uint32_t m_arrayNum{ 0 };
		uint32_t m_endPadding{ 0 };
		std::vector<UniformPack> m_array{};
	};

	/*
	* UniformPack with data type: Mat2Array, Mat3Array, Mat4Array
	*/
	class UnimPackMatArray : public UniformPack
	{
	public:
		UnimPackMatArray(UniformDataType type, uint32_t arrayNum, const std::string &name);
		virtual ~UnimPackMatArray();
		virtual void fillMetaData();
		virtual void calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual uint32_t getArrayNum() { return m_arrayNum; }
		virtual std::string toString() override;

		UnimPackMat &operator[](uint32_t i) { return m_matArray[i]; }

	private:
		static UniformDataType GetArrayDataType(UniformDataType type);

	protected:
		uint32_t m_arrayNum{ 0 };
		uint32_t m_endPadding{ 0 };
		std::vector<UnimPackMat> m_matArray{};
	};

	class UnimPackStruct : public UniformPack
	{
	public:
		UnimPackStruct();
		UnimPackStruct(const std::string &name);
		virtual ~UnimPackStruct();
		virtual void fillMetaData();
		virtual void calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual std::string toString() override;

		void put(uint32_t index, const Ref<UniformPack> &data) { m_data[index] = data; }
		Ref<UniformPack> getData(uint32_t index) { return m_data[index]; }
		std::map<uint32_t, Ref<UniformPack>>::iterator begin() { return m_data.begin(); }
		std::map<uint32_t, Ref<UniformPack>>::iterator end() { return m_data.end(); }

	private:
		uint32_t m_startPadding{ 0 }, m_endPadding{ 0 };
		std::map<uint32_t, Ref<UniformPack>, std::less<uint32_t>> m_data{};
	};

	/*
	* UniformPack with data type: StructArray
	*/
	class UnimPackStructArray : public UniformPack
	{
	public:
		UnimPackStructArray(const std::string &name);
		virtual ~UnimPackStructArray();
		virtual void fillMetaData();
		virtual void calculateOffset(uint32_t byteUsed, uint32_t offset, bool blockStart) override;
		virtual std::string toString() override;

		void pushBack(const UnimPackStruct &data) { m_structArray.push_back(data); m_baseAlign = 0; }
		void pushBack(UnimPackStruct &&data) { m_structArray.push_back(std::forward<UnimPackStruct>(data)); m_baseAlign = 0; }

		UnimPackStruct &operator[](uint32_t i) { return m_structArray[i]; }

	protected:
		uint32_t m_endPadding{ 0 };
		std::vector<UnimPackStruct> m_structArray{};
	};

	/*
	* Example of calculation of layout std140
	*
	* layout (std140) uniform ExampleBlock0
	* {
	*                       // Base Alignment // Byte Used             // Align Offset
	*      float value;     // 4              // 4                     // 0
	*      vec3 vector;     // 16             // 12                    // 16  (multuple times of 16, so 4->16)
	*      mat4 matrix;     // 16             // 16                    // 32  (column 0)
	*                       //                // 16                    // 48  (column 1)
	*                       //                // 16                    // 64  (column 2)
	*                       //                // 16                    // 80  (column 3)
	*                       //                // 0 (end padding)       // 96
	*      float values[3]; // 16             // 4                     // 96  (values[0])
	*                       //                // 4                     // 112 (values[1])
	*                       //                // 4                     // 128 (values[2])
	*                       //                // 0 (end padding)       // 144
	*      bool boolean;    // 4              // 4                     // 144
	*      int integer;     // 4              // 4                     // 148
	* };
	*
	* layout (std140) uniform ExampleBlock1
	* {
	*                       // Base Alignment  // Byte Used            // Align Offset
	*      float a;         // 4               // 4                    // 0
	*      vec2 b;          // 8               // 8                    // 8
	*      vec3 c;          // 16              // 12                   // 16
	*      struct  {        // 16              // 0 (start padding)    // 32
	*          int d;       // 4               // 4                    // 32
	*      	   bvec2 e;     // 8               // 8                    // 40
	*      } f;             // 16              // 0 (end padding)      // 48
	*      float g;         // 4               // 4                    // 48
	*      float h[2];      // 16              // 4                    // 64
	*                                          // 4                    // 80
	*                                          // 0 (end padding)      // 96
	* }
	*/
	class UniformBlock
	{
	public:
		UniformBlock();
		~UniformBlock();

		void calculate();
		void pushBack(const std::string &key, const Ref<UniformPack> &data) { m_blockItems.add(key, data); }
		std::string toString();

	public:
		uint32_t getAlignOffset() const { return m_alignOffset; }
		uint32_t getByteUsed() const { return m_byteUsed; }
		Ref<Buffer> getBuffer() const { return m_buffer; }
		Ref<UniformPack> operator[](const std::string &key) { return m_blockItems.get(key); }

	public:
		static Ref<UniformPack> CreateData(UniformDataType type, const std::string &name, uint32_t arrayNum = 0);

	private:
		uint32_t m_byteUsed{ 0 }, m_alignOffset{ 0 };
		// items in LinkedMap are sorted in insertion order
		LinkedMap<std::string, Ref<UniformPack>> m_blockItems{};
		Ref<Buffer> m_buffer{};

	private:
		friend struct CameraUD;
	};

	template<>
	bool UniformPack::is<UnimPackMatArray>();

	template<>
	bool UniformPack::is<UnimPackStruct>();

	template<>
	bool UniformPack::is<UnimPackStructArray>();
}

#endif