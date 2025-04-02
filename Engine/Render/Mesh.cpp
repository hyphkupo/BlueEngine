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
		// �Ķ���� ����.
		this->vertices.assign(vertices.begin(), vertices.end());	// assign�� ���� ���Ҵ� Ƚ���� �پ�� (for ���� ������ �� emplace_back�� �� ������ ���Ҵ�)
		stride = Vertex::Stride();
		this->indices.assign(indices.begin(), indices.end());

		// @Temp: �ӽ� ���ҽ� ����.
		// ����(Buffer) - �޸� ���.
		D3D11_BUFFER_DESC vertexbufferDesc = {};
		vertexbufferDesc.ByteWidth = stride * (uint32)vertices.size();
		vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// ���� ������.
		D3D11_SUBRESOURCE_DATA vertexData = {};
		// D3D11_SUBRESOURCE_DATA: ���� �����͸� ���� �ϳ��� ���Ƴ��� �� ���
		vertexData.pSysMem = vertices.data();

		// ��ġ ������.
		ID3D11Device& device = Engine::Get().Device();

		// (����)���� ����
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

		// �ε���(����) ���� ����(������ �����ϴ� ����).
		//int indices[] = { 0, 1, 2 };	// �ð� ����(�޼� ��ǥ��)
		// ������ Ȯ���� �� cos ��� => ��?

		// @Temp: �ӽ� ���ҽ� ����.
		// ����(Buffer) - �޸� ���.
		D3D11_BUFFER_DESC indexbufferDesc = {};
		indexbufferDesc.ByteWidth = sizeof(uint32) * (uint32)indices.size();
		indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// ������ ��� ������ ������� ��

		// ���� ������.
		D3D11_SUBRESOURCE_DATA indexData = {};		// ���� ������
		// D3D11_SUBRESOURCE_DATA: ���� �����͸� ���� �ϳ��� ���Ƴ��� �� ���
		indexData.pSysMem = indices.data();

		result = device.CreateBuffer(		// ������ �޾����Ƿ� -> ��� .
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
		// ���ҽ� ����.
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
		// (���ε��� ����) ���ؽ�Ʈ ������.
		static ID3D11DeviceContext& context = Engine::Get().Context();	// �� �����Ӹ��� ȣ��� ���ɼ� �����Ƿ� ���� ���� static���� ����

		// ����/�ε��� ���� ���ε�.
		static uint32 offset = 0;
		context.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);	// IA�ܰ� (v.s �� ���� ���� �ܰ�)
		context.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void MeshData::UpdateVertexBuffer(const std::vector<Vertex>& vertices)
	{
		// �Ķ���� ����.
		this->vertices.assign(vertices.begin(), vertices.end());

		// ���� ���۰� ������, ���� �� �����.
		if (vertexBuffer)
		{
			vertexBuffer->Release();
			vertexBuffer = nullptr;
		}

		D3D11_BUFFER_DESC vertexbufferDesc = {};
		vertexbufferDesc.ByteWidth = stride * (uint32)vertices.size();
		vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// ���� ������.
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices.data();

		// ��ġ ������.
		ID3D11Device& device = Engine::Get().Device();

		// (����)���� ����
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
	}

	Mesh::Mesh()
	{
	}

	uint32 Mesh::SubMeshCount() const
	{
		return (uint32)meshes.size();
	}

	std::weak_ptr<MeshData> Mesh::GetSubMesh(int index) const
	{
		// ���� ó��.
		if (index < 0 || index > (int)meshes.size())
		{
			return std::weak_ptr<MeshData>();
		}

		return meshes[index];
	}

	//void Mesh::Draw()
	//{
	//	// ���ؽ�Ʈ ������.
	//	static ID3D11DeviceContext& context = Engine::Get().Context();

	//	// Ʈ������ ���ε�.
	//	transform.Bind();

	//	// ���� ��ȸ�ϸ鼭 ���ε� & ��ο�.
	//	for (int ix = 0; ix < (int32)meshes.size(); ++ix)
	//	{
	//		//auto shader = shaders[ix].lock();
	//		//if (!shader)
	//		//{
	//		//	continue;
	//		//	//shader->Bind();
	//		//}

	//		// ���� ���ҽ� ��������.
	//		auto mesh = meshes[ix].lock();
	//		auto shader = shaders[ix].lock();

	//		// ���ҽ��� ������ ������ �׸���.
	//		if (mesh && shader)
	//		{
	//			mesh->Bind();
	//			shader->Bind();
	//			context.DrawIndexed(mesh->IndexCount(), 0, 0);	// ��ο� ��
	//		}

	//		//meshes[ix]->Bind();
	//		//shaders[ix].lock()->Bind();
	//		//context.DrawIndexed(meshes[ix]->IndexCount(), 0, 0);	// ��ο� ��
	//	}
	//}
}