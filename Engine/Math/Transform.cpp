#include "Transform.h"
#include "core/Engine.h"
#include "core/Common.h"

namespace Blue
{
	Transform::Transform()
	{
		// Ʈ������ ��� ���(SRT).
		transformMatrix = 
			Matrix4::Scale(scale) 
			* Matrix4::Rotation(rotation) 
			* Matrix4::Translation(position);

		// ��ġ ��� (CPU�� GPU�� ����� �ٷ�� ����� �޶�).
		// ����� ����� ������ ��ķ� ��ȯ�ϱ� ���� ��ġ��� ó��.
		transformMatrix = Matrix4::Transpose(transformMatrix);

		// ��� ����.
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = Matrix4::Stride();
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		// ���ۿ� ���� ������ ����.
		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = &transformMatrix;

		// ���� ����.
		ID3D11Device& device = Engine::Get().Device();
		ThrowIfFailed(
			device.CreateBuffer(&bufferDesc, &bufferData, &constantBuffer),
			TEXT("Failed to create a transform buffer."));
	}

	Transform::~Transform()
	{
		// ���ҽ� ����.
		if (constantBuffer)
		{
			constantBuffer->Release();
			constantBuffer = nullptr;
		}
	}

	void Transform::Bind()
	{
		static ID3D11DeviceContext& context = Engine::Get().Context();

		// ������ ������Ʈ.
		// Ʈ������ ��� ���(SRT).
		transformMatrix =
			Matrix4::Scale(scale)
			* Matrix4::Rotation(rotation)
			* Matrix4::Translation(position);

		// ��ġ ��� (CPU�� GPU�� ����� �ٷ�� ����� �޶�).
		// ����� ����� ������ ��ķ� ��ȯ�ϱ� ���� ��ġ��� ó��.
		transformMatrix = Matrix4::Transpose(transformMatrix);

		// ���� ������Ʈ.
		context.UpdateSubresource(constantBuffer, 0, nullptr, &transformMatrix, 0, 0);

		// ���� ���ε�.
		context.VSSetConstantBuffers(0, 1, &constantBuffer);	// VS�� �����Ƿ� ���� ���̴�
	}
}