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

		// �޸� �ʺ�(����: ����Ʈ).
		static uint32 Stride() { return sizeof(Matrix4); }

		// Degree <-> Radian.
		static Matrix4 Identity;
		static constexpr float degreeToRadian = 3.141592f / 180.0f;		// constexpr: �̸� ��� �� ������ �Ŀ� ��� �˷��� ?
		static constexpr float radianToDegree = 180.0f / 3.141592f;

	private:
		// ����ü.	=> �޸𸮸� ������ (����ü ������ ���� ū �޸��� ������ �������� ������)
		union
		{
			struct	// ������ �ǹ� x, ������ ���� �뵵
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};

			float elements[4 * 4];		// [4][4]�� �޸� ������ ����
		};
	};
}