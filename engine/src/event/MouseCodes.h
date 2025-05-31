#pragma once

#include <atomic>

namespace pio
{
	using MouseCode = uint16_t;

	class Mouse
	{
	public:
		enum : MouseCode // From glfw3.h
		{			
			Button0                = 0,
			Button1                = 1,
			Button2                = 2,
			Button3                = 3,
			Button4                = 4,
			Button5                = 5,
			Button6                = 6,
			Button7                = 7,
			Button8                = 8,
			ButtonRelease          = 9,
			// -----------------------------
			ButtonLeft             = Button0,
			ButtonRight            = Button1,
			ButtonMiddle           = Button2			
		};
	public:
		static void Set(MouseCode code) { k_MouseCode = code; }
		static void SetPosition(float x, float y) { k_PosX = x; k_PosY = y; }
		static MouseCode Status() { return k_MouseCode; }
		static void Release() { Set(Mouse::ButtonRelease); }
		static bool IsLeftBtnDown() { return k_MouseCode == ButtonLeft; }
		static bool IsRightBtnDown() { return k_MouseCode == ButtonRight; }
		static bool IsMiddleBtnDown() { return k_MouseCode == ButtonMiddle; }
		static bool IsBtnReleased() { return k_MouseCode == ButtonRelease; }
		static float PosX() { return k_PosX; }
		static float PosY() { return k_PosY; }
		 
	private:
		Mouse() {}
		~Mouse() = default;

	private:
		static std::atomic<MouseCode> k_MouseCode;
		static std::atomic<float> k_PosX, k_PosY;
	};
}