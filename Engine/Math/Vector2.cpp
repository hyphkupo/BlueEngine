#include "Vector2.h"
#include <Windows.h>
#include <cmath>
#include <cassert>

namespace Blue
{
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
	const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);

	std::wstring Vector2::ToString()
	{
		wchar_t buffer[256];
		swprintf_s(buffer, 256, TEXT("(%f,%f)"), x, y);

		return buffer;
	}

	float Vector2::Length()
	{
		return sqrtf(x * x + y * y);	// 이 계산이 무거워서 LengthSquared를 쓰기도 함
	}

	Vector2 operator+(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x + right.x, left.y + right.y);
	}

	Vector2 operator-(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x - right.x, left.y - right.y);
	}

	Vector2 operator*(const Vector2& vector, float scale)
	{
		return Vector2(vector.x * scale, vector.y * scale);
	}

	Vector2 operator*(float scale, const Vector2& vector)
	{
		//operator*(vector, scale);
		return vector * scale;
	}

	Vector2 operator/(const Vector2& vector, float scale)
	{
		assert(scale != 0.0f);
		return Vector2(vector.x / scale, vector.y / scale);
	}

	float Dot(const Vector2& left, const Vector2& right)	// friend 붙인 외부함수 => Vector2:: 붙이지 않음
	{
		return (left.x * right.x) + (left. y * right.y);
	}

	Vector2 Lerp(const Vector2& from, const Vector2& to, float t)
	{
		if (t < 0.0f)
		{
			t = 0.0f;
		}
		if (t > 1.0f)
		{
			t = 1.0f;
		}

		return (1.0f - t) * from + t * to;
	}

	Vector2 Vector2::Normalized()
	{
		float length = Length();
		return Vector2(x / length, y / length);
	}

	bool Vector2::Equals(const Vector2& other)
	{
		return x == other.x && y == other.y;	// float는 오차 발생 가능 => 차 구해서 차의 절댓값이 epsilon보다 작거나 같으면 같다고 판단 ...
	}

	Vector2& Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vector2& Vector2::operator/=(float scale)
	{
		// 나눌 값이 0이 아닌지 확인하는 것이 좋음.
		assert(scale != 0.0f);

		x /= scale;
		y /= scale;
		return *this;
	}

	bool Vector2::operator==(const Vector2& other)
	{
		return x == other.x && y == other.y;
	}

	bool Vector2::operator!=(const Vector2& other)
	{
		return x != other.x && y != other.y;
	}

	Vector2 Vector2::operator-() const
	{
		return Vector2(-x, -y);
	}
}