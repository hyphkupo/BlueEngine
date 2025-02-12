#pragma once

// DirectX11 ���.
#include <d3d11.h>
#include <dxgi.h>
#include <memory>	// ����Ʈ �����͸� ���� ���� ���

#include "../Core/Type.h"

namespace Blue
{
	// �׷���ī�忡�� ó���ϴ� ��/���ҽ��� �����ϴ� Ŭ����.
	// RHI - Render Hardware Interface.
	class Renderer
	{
		// ���� Ŭ���� friend ����.
		friend class Engine;

	public:
		Renderer(uint32 width, uint32 height, HWND window);
		~Renderer();

		// �׸��� �Լ�.
		void Draw();

	private:
		// ���ҽ�.
		// ��ġ��.
		ID3D11Device* device = nullptr;				// ���ҽ� ����
		ID3D11DeviceContext* context = nullptr;		// ��ġ�� ������ �� ���ҽ��� �׷���ī�忡 ����, drawcall (�׷���ī�忡 ����, ���)
		IDXGISwapChain* swapChain = nullptr;

		// ����.
		ID3D11RenderTargetView* renderTargetView = nullptr;

		// ����Ʈ.	(ȭ��)
		D3D11_VIEWPORT viewport;

		// @Temp: �ӽ�.
		// ���� ���� (���� ������ �����ϴ� �� �����).

		// �ε��� ���� (������ ������ �� ������ ������ ����).
		
		// ���̴� ��ü.
		//std::unique_ptr<class Shader> shader;

		// �ﰢ�� �޽� ��ü.
		std::unique_ptr<class TriangleMesh> mesh;

		//// �Է� ���̾ƿ�.
		//ID3D11InputLayout* inputlayout = nullptr;

		//// ���̴� ��ü.
		//ID3D11VertexShader* vertexShader = nullptr;
		//ID3D11PixelShader* pixelShader = nullptr;
	};
}