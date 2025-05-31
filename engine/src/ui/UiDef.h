#pragma once

#include "event/Event.h"

namespace pio
{
	struct Rect2D
	{
		uint32_t Left{ 0 };
		uint32_t Top{ 0 };
		uint32_t Right{ 0 };
		uint32_t Bottom{ 0 };

		Rect2D() {}
		Rect2D(uint32_t l, uint32_t t, uint32_t r, uint32_t b) : Left(l), Top(t), Right(r), Bottom(b) {}
		Rect2D(const glm::ivec2 lt, uint32_t width, uint32_t height) : Left(lt.x), Top(lt.y), Right(lt.x + width), Bottom(lt.y + height) {}

		uint32_t Width()  const { return Right - Left; }
		uint32_t Height() const { return Bottom - Top; }
		glm::vec2 Center() const { return glm::vec2(Left + Width() * 0.5f, Top + Height() * 0.5f); }
		bool Contain(uint32_t x, uint32_t y) const { return x >= Left && x <= Right && y >= Top && y <= Bottom; }

		bool operator==(const Rect2D& rhs) const
		{
			if(this == &rhs) 
				return true;
			return this->Left == rhs.Left && this->Top == rhs.Top && this->Right == rhs.Right && this->Bottom == rhs.Bottom;
		}
		bool operator!=(const Rect2D& rhs) const { return !((*this) == rhs); }

		bool Intersect(const Rect2D& rhs)
		{
			glm::uvec2 lt = glm::uvec2(std::max(Left, rhs.Left), std::max(Top, rhs.Top));
			glm::uvec2 rb = glm::uvec2(std::min(Right, rhs.Right), std::min(Bottom, rhs.Bottom));
			return lt.x <= rb.x && lt.y <= rb.y;
		}

		void DoUnion(const Rect2D& rhs)
		{
			if(this != &rhs && Intersect(rhs))
			{
				Left = std::min(Left, rhs.Left);
				Top = std::min(Top, rhs.Top);
				Right = std::max(Right, rhs.Right);
				Bottom = std::max(Bottom, rhs.Bottom);
			}
		}
	};

	struct LayoutRatio
	{
		float Left{ 0.f };  // 0.f - 1.f
		float Top{ 0.f };   // 0.f - 1.f
		float Right{ 1.f }; // 0.f - 1.f
		float Bottom{ 1.f };// 0.f - 1.f

		LayoutRatio() {}
		LayoutRatio(float l, float t, float r, float b) : 
			Left(Math::Clamp(l, 0.f, 1.f)), 
			Top(Math::Clamp(t, 0.f, 1.f)), 
			Right(Math::Clamp(r, 0.f, 1.f)), 
			Bottom(Math::Clamp(b, 0.f, 1.f))
		{
		}

		bool operator==(const LayoutRatio& rhs) const
		{
			return (this != &rhs) && Math::Equal(Left, rhs.Left) && Math::Equal(Top, rhs.Top) &&
				Math::Equal(Right, rhs.Right) && Math::Equal(Bottom, rhs.Bottom);
		}

		bool operator!=(const LayoutRatio& rhs) const { return !((*this) == rhs); }
	};

	/*
	* @brief: LayoutParams is mainly used for layout in the parent widget.
	*		  LayoutParams.Ratio defines the rect in parent widget by ratio in [0, 1].
	*		  LayoutParams.Rect is the pixel result of Ratio with origin on the left-top.
	*/
	struct LayoutParams
	{
		LayoutRatio Ratio{};
		Rect2D Rect{};

		LayoutParams() {}
		LayoutParams(float l, float t, float r, float b) : Ratio(l, t, r, b) {}
		LayoutParams(uint32_t w, uint32_t h, float l, float t, float r, float b) : Ratio(l, t, r, b) { Calculate(w, h); }

		bool operator==(const LayoutParams& rhs) const { return this != &rhs && Ratio == rhs.Ratio && Rect == rhs.Rect; }
		bool operator!=(const LayoutParams& rhs) const { return !((*this) == rhs); }

		void Calculate(uint32_t w, uint32_t h)
		{
			Rect.Left = w * Ratio.Left;
			Rect.Top = h * Ratio.Top;
			Rect.Right = w * Ratio.Right;
			Rect.Bottom = h * Ratio.Bottom;
		}
	};

	class NodeSelectEvent : public Event
	{
		RTTR_ENABLE(Event)
	public:
		NodeSelectEvent(uint32_t idx) : Event(EVENT_ID_ON_NODE_SELECT), Select(idx) {}

		EVENT_CLASS_TYPE(Custom)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	public:
		uint32_t Select { InvalidId };
	};
}