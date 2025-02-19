#include "Mesh.h"
#include "../Core/Engine.h"
#include "../Shader/Shader.h"

namespace Blue
{
	MeshData::MeshData()
	{
	}

	MeshData::MeshData(
		const std::vector<Vertex>& vertices, 
		const std::vector<uint32>& indices)
	{
		// 파라미터 복사.
		this->vertices.assign(vertices.begin(), vertices.end());	// assign을 쓰면 재할당 횟수가 줄어듦 (for 루프 돌렸을 때 emplace_back은 쓸 때마다 재할당)
		stride = Vertex::Stride();
		this->indices.assign(indices.begin(), indices.end());

		// @Temp: 임시 리소스 생성.
		// 버퍼(Buffer) - 메모리 덩어리.
		D3D11_BUFFER_DESC vertexbufferDesc = {};
		vertexbufferDesc.ByteWidth = stride * (uint32)vertices.size();
		vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// 정점 데이터.
		D3D11_SUBRESOURCE_DATA vertexData = {};
		// D3D11_SUBRESOURCE_DATA: 여러 데이터를 버퍼 하나에 몰아넣을 때 사용
		vertexData.pSysMem = vertices.data();

		// 장치 얻어오기.
		ID3D11Device& device = Engine::Get().Device();

		// (정점)버퍼 생성
		auto result = device.CreateBuffer(
			&vertexbufferDesc, &vertexData, &vertexBuffer);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create vertex buffer.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 인덱스(색인) 버퍼 생성(정점을 조립하는 순서).
		//int indices[] = { 0, 1, 2 };	// 시계 방향(왼손 좌표계)
		// 방향을 확인할 때 cos 사용 => 왜?

		// @Temp: 임시 리소스 생성.
		// 버퍼(Buffer) - 메모리 덩어리.
		D3D11_BUFFER_DESC indexbufferDesc = {};
		indexbufferDesc.ByteWidth = sizeof(uint32) * (uint32)indices.size();
		indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 버퍼의 사용 목적도 적어줘야 함

		// 정점 데이터.
		D3D11_SUBRESOURCE_DATA indexData = {};		// 실제 데이터
		// D3D11_SUBRESOURCE_DATA: 여러 데이터를 버퍼 하나에 몰아넣을 때 사용
		indexData.pSysMem = indices.data();

		result = device.CreateBuffer(		// 참조로 받았으므로 -> 대신 .
			&indexbufferDesc, &indexData, &indexBuffer);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create index buffer.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}
	}

	MeshData::~MeshData()
	{
		// 리소스 해제.
		if (vertexBuffer)
		{
			vertexBuffer->Release();
		}
		if (indexBuffer)
		{
			indexBuffer->Release();
		}
	}

	void MeshData::Bind()
	{
		// (바인딩을 위해) 컨텍스트 얻어오기.
		static ID3D11DeviceContext& context = Engine::Get().Context();	// 매 프레임마다 호출될 가능성 높으므로 방지 위해 static으로 선언

		// 정점/인덱스 버퍼 바인딩.
		static uint32 offset = 0;
		context.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);	// IA단계 (v.s 전 정점 조립 단계)
		context.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	Mesh::Mesh()
	{
	}

	void Mesh::Draw()
	{
		// 컨텍스트 얻어오기.
		static ID3D11DeviceContext& context = Engine::Get().Context();

		// 루프 순회하면서 바인딩 & 드로우.
		for (int ix = 0; ix < (int32)meshes.size(); ++ix)
		{
			meshes[ix]->Bind();
			shaders[ix]->Bind();
			context.DrawIndexed(meshes[ix]->IndexCount(), 0, 0);
		}
	}
}