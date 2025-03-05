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

		// �̵� ��ȯ ���(T).
		static Matrix4 Translation(const Vector3& position);
		static Matrix4 Translation(float x, float y, float z);

		// ȸ�� ��ȯ ���(R).
		static Matrix4 Rotation(const Vector3& rotation);
		static Matrix4 Rotation(float x, float y, float z);
		static Matrix4 RotationX(float angle);
		static Matrix4 RotationY(float angle);
		static Matrix4 RotationZ(float angle);

		// ũ�� ��ȯ ���(S).
		static Matrix4 Scale(const Vector3& scale);
		static Matrix4 Scale(float x, float y, float z);
		static Matrix4 Scale(float scale);

		// ��ġ ���.
		static Matrix4 Transpose(const Matrix4& target);

		// operator overloading.
		Matrix4& operator=(const Matrix4& other);
		Matrix4 operator*(const Matrix4& other);
		Matrix4 operator*=(const Matrix4& other);

		friend Vector3 operator*(const Matrix4& matrix, const Vector3& vector);
		friend Vector3 operator*(const Vector3& vector, const Matrix4& matrix);

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