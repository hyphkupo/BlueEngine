#include "Vector2.h"
#include <Windows.h>
#include <cmath>

namespace Blue
{
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
	const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);

	std::wstring Vector2::ToString()
	{
		wchar_t buffer[20];
		swprintf_s(buffer, 256, TEXT("(%f,%f)"), x, y);

		return buffer;
	}

	float Vector2::Length()
	{
		return sqrtf(x * x + y * y);	// 이 계산이 무거워서 LengthSquared를 쓰기도 함
	}

	float Vector2::Dot(const Vector2& other)
	{
		return (x * other.x) + (y * other.y);
	}

	Vector2 Vector2::Normalized()
	{
		float length = Length();
		return Vector2(x / length, y / length);
	}

	bool Vector2::Equals(const Vector2& other)
	{
		return x == other.x && y == other.y;	// float는 오차 발생 가능 => 차(eliption?) 구해서 차의 절댓값이 오차범위 내면 같다고 판단 ...
	}
}