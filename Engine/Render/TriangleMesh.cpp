#include "TriangleMesh.h"
#include "Vertex.h"
#include "../Shader/Shader.h"

namespace Blue
{
	TriangleMesh::TriangleMesh()
	{
		// 정점 배열.
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3(0.0f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.5f, 0.0f)),
			Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),
			Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)),
		};

		// 인덱스 배열.
		std::vector<uint32> indices = { 0, 1, 2 };	// 이 순서대로 그림
		
		meshes.emplace_back(std::make_shared<MeshData>(vertices, indices));	// 모델링 / 서브메쉬가 여러개면 이 코드 여러번 사용
		shaders.emplace_back(std::make_shared<Shader>());	// 머티리얼(Shader)
	}
}