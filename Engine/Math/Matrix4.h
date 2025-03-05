#pragma once

#include "Core/Type.h"
#include "Vector3.h"

namespace Blue
{
	class Matrix4
	{
	public:
		Matrix4();
		Matrix4(const Matrix4& other);
		~Matrix4() = default;

		// 메모리 너비(단위: 바이트).
		static uint32 Stride() { return sizeof(Matrix4); }

		// Degree <-> Radian.
		static Matrix4 Identity;
		static constexpr float degreeToRadian = 3.141592f / 180.0f;		// constexpr: 미리 계산 후 컴파일 후에 결과 알려줌 ?
		static constexpr float radianToDegree = 180.0f / 3.141592f;

	private:
		// 공용체.	=> 메모리를 공유함 (공용체 내에서 가장 큰 메모리의 공간을 나머지가 공유함)
		union
		{
			struct	// 문법적 의미 x, 범위를 묶는 용도
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};

			float elements[4 * 4];		// [4][4]와 메모리 구조가 같음
		};
	};
}