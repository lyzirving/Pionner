#pragma once

#include "UniformStorageLayout.h"

namespace pio
{
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

		void Calculate();
		void PushBack(const std::string& key, const Ref<UniformStorageLayout>& data) { m_BlockItems.Add(key, data); }
		void WriteAt(const void* data, uint64_t size, uint64_t offset);
		std::string ToString();

	public:
		bool AnyChange() const { return m_bDirty; }
		void Invalidate(bool val = true) { m_bDirty = val; }
		uint32_t GetAlignOffset() const { return m_AlignOffset; }
		uint32_t GetByteUsed() const { return m_ByteUsed; }
		Ref<Buffer> GetBuffer() const { return m_Buffer; }
		Ref<UniformStorageLayout> operator[](const std::string& key) { return m_BlockItems.Get(key); }

	public:
		static Ref<UniformStorageLayout> CreateData(UniformType type, const std::string& name, uint32_t arrayNum = 0);

	private:
		uint32_t m_ByteUsed{ 0 }, m_AlignOffset{ 0 };
		// items in LinkedMap are sorted in insertion order
		LinkedMap<std::string, Ref<UniformStorageLayout>> m_BlockItems{};
		Ref<Buffer> m_Buffer{};
		bool m_bDirty{ true };
	};
}