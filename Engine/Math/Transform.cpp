#include "Transform.h"
#include "core/Engine.h"
#include "core/Common.h"

namespace Blue
{
	Transform::Transform()
	{
		// 트랜스폼 행렬 계산(SRT).
		transformMatrix = 
			Matrix4::Scale(scale) 
			* Matrix4::Rotation(rotation) 
			* Matrix4::Translation(position);

		// 전치 행렬 (CPU와 GPU가 행렬을 다루는 방식이 달라서).
		// 행기준 행렬을 열기준 행렬로 변환하기 위해 전치행렬 처리.
		transformMatrix = Matrix4::Transpose(transformMatrix);

		// 상수 버퍼.
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = Matrix4::Stride();
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;					// 빈번하게 바뀔 값이라고 선언
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPU가 쓰기 권한을 얻게 설정 (GPU는 읽기만 하면 됨)

		// 버퍼에 담을 데이터 설정.
		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = &transformMatrix;

		// 버퍼 생성.
		ID3D11Device& device = Engine::Get().Device();
		ThrowIfFailed(
			device.CreateBuffer(&bufferDesc, &bufferData, &constantBuffer),
			TEXT("Failed to create a transform buffer."));
	}

	Transform::~Transform()
	{
		// 리소스 해제.
		if (constantBuffer)
		{
			constantBuffer->Release();
			constantBuffer = nullptr;
		}
	}

	void Transform::Tick()
	{
		// 데이터 업데이트.
		// 트랜스폼 행렬 계산(SRT).
		transformMatrix =
			Matrix4::Scale(scale)
			* Matrix4::Rotation(rotation)
			* Matrix4::Translation(position);

		// 전치 행렬 (CPU와 GPU가 행렬을 다루는 방식이 달라서).
		// 행기준 행렬을 열기준 행렬로 변환하기 위해 전치행렬 처리.
		transformMatrix = Matrix4::Transpose(transformMatrix);
	}

	void Transform::Bind()
	{
		static ID3D11DeviceContext& context = Engine::Get().Context();

		// 버퍼 업데이트.
		//context.UpdateSubresource(constantBuffer, 0, nullptr, &transformMatrix, 0, 0);
		D3D11_MAPPED_SUBRESOURCE resource = {};
		// 리소스에 스레드 lock을 걺 (cpu가 리소스를 다 쓰기 전까지, 업데이트를 다 하기 전까지 gpu가 건드리지/가져가지 못하도록)
		context.Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);	// 기존에 썼던 데이터 폐기
		//resource.pData = &transformMatrix;
		memcpy(resource.pData, &transformMatrix, sizeof(Matrix4));	// 이게 좀 더 빠름
		context.Unmap(constantBuffer, 0);

		// 버퍼 바인딩.
		context.VSSetConstantBuffers(0, 1, &constantBuffer);	// VS가 들어갔으므로 정점 셰이더
	}
}