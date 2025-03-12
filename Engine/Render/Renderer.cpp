#include "Renderer.h"
#include <vector>
#include <d3dcompiler.h>	// ���̴� ó���� ���� �߰�

#include "../Math/Vector3.h"
//#include "../Shader/Shader.h"
#include "TriangleMesh.h"
#include "QuadMesh.h"
#include "Core/Common.h"

namespace Blue
{
	Renderer::Renderer(uint32 width, uint32 height, HWND window)
	{
		// ��ġ ������ ����ϴ� �ɼ�.
		uint32 flag = 0u;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// ������ ���̺귯�� ����.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,		// �׷���ī�尡 �� ���� ���� �� �ϸ� ���� �� ��� , ... �ݺ�
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL outFeatureLevel;

		// ��ġ ����.
		//D3D11CreateDevice(
		//	nullptr,
		//	D3D_DRIVER_TYPE_HARDWARE,
		//	nullptr,
		//	flag,
		//	featureLevels,
		//	_countof(featureLevels),
		//	D3D11_SDK_VERSION,
		//	&device,
		//	&outFeatureLevel,
		//	&context
		//);

		//���� ü�� ���� ����ü.
		//DXGI_MODE_DESC BufferDesc;
		//DXGI_SAMPLE_DESC SampleDesc;
		//DXGI_USAGE BufferUsage;
		//UINT BufferCount;
		//HWND OutputWindow;
		//BOOL Windowed;
		//DXGI_SWAP_EFFECT SwapEffect;
		//UINT Flags;

		// ��Ƽ ���ø� ���� ���� Ȯ��.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// â ���?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;		// ����� ����.	=> �Ϲ������� ������ �þ���� â �ٲ� �� �ε巯��
		swapChainDesc.SampleDesc.Count = 1;	// ��Ƽ ���ø� ����. (��Ƽ ���ø� ���� 1�� => ��Ƽ ���ø��� �� �ϰڴ�, �� ���ڴٴ� ��)
		swapChainDesc.SampleDesc.Quality = 0;	// ��Ƽ ���ø� ����. (���ø� ���� ���� count - 1)
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 8bit 4���̹Ƿ� 32bit, unsigned(��ȣ����), normalize(����ȭ�� => 0~1������ ������ ���ν�Ű�ڴ�)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// D3D_FEATURE_LEVEL targetLevel;

		// ��ġ ����.
		//ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		//	nullptr,
		//	D3D_DRIVER_TYPE_HARDWARE,
		//	nullptr,
		//	flag,
		//	featureLevels,
		//	_countof(featureLevels),	// �迭�� ����
		//	D3D11_SDK_VERSION,
		//	&swapChainDesc,
		//	&swapChain,	// ���� ��ȯ
		//	&device,	// ���ҽ� ���� (CPU)
		//	nullptr,	// nullptr ��� D3D_FEATURE_LEVEL ���� ���� ������ ���� �´��� Ȯ��...?
		//	&context	// ���ε� (GPU�� �ѱ�) => ����, DRAW
		//	// dx�� ���� set�̶�� �̸��� ���� (����, ����, ���ε�)
		//), TEXT("Failed to create devices."));

		// ��ġ ����.
		ThrowIfFailed(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),	// �迭�� ����
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&context
		), TEXT("Failed to create devices."));

		// IDXGIFactory ���ҽ� ����.
		IDXGIFactory* factory = nullptr;
		//CreateDXGIFactory(__uuidof(factory), reinterpret_cast<void**>(&factory));
		ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)),
			TEXT("Failed to create dxgifactory."));

		//ThrowIfFailed(D3D11CreateDevice(
		//	nullptr,
		//	D3D_DRIVER_TYPE_HARDWARE,
		//	nullptr,
		//	flag,
		//	featureLevels,
		//	_countof(featureLevels),
		//	/*D3D11_SDK_VERSION*/ 1,
		//	&device,
		//	nullptr,
		//	&context
		//), TEXT("Failed to create device");
		
		// ��� Ȯ��.
		//if (FAILED(result))		// result < 0�� ����
		//{
		//	MessageBoxA(nullptr, "Failed to create devices.", "Error", MB_OK);		// ~A: �ƽ�Ű ����
		//	__debugbreak();
		//}

		// SwapChain ����.
		ThrowIfFailed(factory->CreateSwapChain(
			device,
			&swapChainDesc,
			&swapChain
		), TEXT("Failed to create a swap chain."));

		// ���� Ÿ�� �� ����.		// �� => ���ҽ� => ���ҽ��� ��� (������ ����!)
		ID3D11Texture2D* backbuffer = nullptr;
		//swapChain->GetBuffer(
		//	0, 
		//	__uuidof(backbuffer), 
		//	reinterpret_cast<void**>(&backbuffer)
		//);

		ThrowIfFailed(swapChain->GetBuffer(
			0, 
			IID_PPV_ARGS(&backbuffer)
		), TEXT("Failed to get back buffer"));

		ThrowIfFailed(device->CreateRenderTargetView(
			backbuffer, nullptr, &renderTargetView
		), TEXT("Failed to create render target views"));

		// ���� Ÿ�� �� ���ε�(����).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);		// OutputMerger (=> ���� ��� �ܰ迡 ����� �Լ���) => render target �ѱ�

		// ����Ʈ(ȭ��).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// ����Ʈ ����.
		context->RSSetViewports(1, &viewport);		// rs(�����Ͷ�����) �ܰ迡�� ����Ѵ�

		// ���� ������ ����.
		//std::vector
		// vertex -> vertices.
		Vector3 vertices[] =
		{
			Vector3(0.0f, 0.5f, 0.5f),
			Vector3(0.5f, -0.5f, 0.5f),
			Vector3(-0.5f, -0.5f, 0.5f),
		};

		/*
		//// @Temp: �ӽ� ���ҽ� ����.
		//// ����(Buffer) - �޸� ���.
		//D3D11_BUFFER_DESC vertexbufferDesc = {};
		//vertexbufferDesc.ByteWidth = Vector3::Stride() * 3;
		//vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//// ���� ������.
		//D3D11_SUBRESOURCE_DATA vertexData = {};
		//// D3D11_SUBRESOURCE_DATA: ���� �����͸� ���� �ϳ��� ���Ƴ��� �� ���
		//vertexData.pSysMem = vertices;

		//// (����)���� ����
		//device->CreateBuffer(
		//	&vertexbufferDesc, &vertexData, &vertexBuffer);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr, 
		//		"Failed to create vertex buffer.", 
		//		"Error", 
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		//// �ε���(����) ���� ����(������ �����ϴ� ����).
		//int indices[] = { 0, 1, 2 };	// �ð� ����(�޼� ��ǥ��)
		//// ������ Ȯ���� �� cos ��� => ��?

		//// @Temp: �ӽ� ���ҽ� ����.
		//// ����(Buffer) - �޸� ���.
		//D3D11_BUFFER_DESC indexbufferDesc = {};
		//indexbufferDesc.ByteWidth = sizeof(int) * 3;
		//indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// ������ ��� ������ ������� ��

		//// ���� ������.
		//D3D11_SUBRESOURCE_DATA indexData = {};		// ���� ������
		//// D3D11_SUBRESOURCE_DATA: ���� �����͸� ���� �ϳ��� ���Ƴ��� �� ���
		//indexData.pSysMem = indices;

		//result = device->CreateBuffer(
		//	&indexbufferDesc, &indexData, &indexBuffer);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr, 
		//		"Failed to create index buffer.", 
		//		"Error", 
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		//// ���� ���̴� ������(�ؼ�).
		//ID3DBlob* vertexShaderBuffer = nullptr;		// ���̴� ���� �����
		//result = D3DCompileFromFile(
		//	TEXT("VertexShader.hlsl"),
		//	nullptr,
		//	nullptr,
		//	"main",		// pEntrypoint
		//	"vs_5_0",	// pTarget
		//	0, 0,
		//	&vertexShaderBuffer, nullptr
		//);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr,
		//		"Failed to compile vertex shader.",
		//		"Error",
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		//// ���� ���̴� ����.
		//result = device->CreateVertexShader(
		//	vertexShaderBuffer->GetBufferPointer(),
		//	vertexShaderBuffer->GetBufferSize(),
		//	nullptr,
		//	&vertexShader
		//);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr,
		//		"Failed to create vertex shader.",
		//		"Error",
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		//// �Է� ���̾ƿ�.
		//// ���� ���̴��� ������ ���� �����Ͱ� ��� ������� �˷���, ���赵. (���̴����� ����� �����Ϳ� �Ѱ��ִ� ���� �����Ͱ� ���ƾ� ��?)
		////LPCSTR SemanticName;
		////UINT SemanticIndex;
		////DXGI_FORMAT Format;
		////UINT InputSlot;
		////UINT AlignedByteOffset;
		////D3D10_INPUT_CLASSIFICATION InputSlotClass;
		////UINT InstanceDataStepRate;
		//D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		//{
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	// float: 32bit (4���� ���ʹ� �ڿ� a�� ����)
		//	  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		//};

		//result = device->CreateInputLayout(
		//	inputDesc,
		//	1,
		//	vertexShaderBuffer->GetBufferPointer(),
		//	vertexShaderBuffer->GetBufferSize(),
		//	&inputlayout
		//);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr,
		//		"Failed to create input layout.",
		//		"Error",
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		//// �ȼ� ���̴� ������/����.
		//// �� ���ҽ� ���ε�. - x?
		//// �ȼ� ���̴� ������.
		//ID3DBlob* pixelShaderBuffer = nullptr;
		//result = D3DCompileFromFile(
		//	TEXT("PixelShader.hlsl"),
		//	nullptr,
		//	nullptr,
		//	"main",
		//	"ps_5_0",
		//	0, 0,
		//	&pixelShaderBuffer, nullptr
		//);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr,
		//		"Failed to compile pixel shader.",
		//		"Error",
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		//// �ȼ� ���̴� ����.
		//result = device->CreatePixelShader(
		//	pixelShaderBuffer->GetBufferPointer(),
		//	pixelShaderBuffer->GetBufferSize(),
		//	nullptr,
		//	&pixelShader
		//);

		//if (FAILED(result))
		//{
		//	MessageBoxA(
		//		nullptr,
		//		"Failed to create pixel shader.",
		//		"Error",
		//		MB_OK
		//	);

		//	__debugbreak();
		//}

		// ���̴� ��ü ����.
		//shader = std::make_unique<Shader>();
		*/
	}

	Renderer::~Renderer()
	{
		device->Release();				
		context->Release();		
		swapChain->Release();

		// ����.
		renderTargetView->Release();
	}
	
	void Renderer::Draw()
	{
		if (mesh == nullptr)
		{
			mesh = std::make_unique<QuadMesh>();
			mesh->transform.scale = Vector3::One * 0.5f;
			mesh->transform.position.x = 0.5f;
		}

		if (mesh2 == nullptr)
		{
			mesh2 = std::make_unique<QuadMesh>();
			mesh2->transform.scale = Vector3::One * 0.5f;
			mesh2->transform.position.x = -0.5f;
		}

		//if (mesh3 == nullptr)
		//{		
		//	mesh3 = std::make_unique<TriangleMesh>();
		//	mesh3->transform.scale = Vector3::One * 0.5f;
		//	//mesh3->transform.position.y = 0.5f;
		//}

		// �׸��� �� �۾� (BeginScene).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);

		// �����(Clear).			����� �۾��� ��� ���� ... ���� �ϳ��� ���� ��
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);	// �츮�� �̹����� ����ϴ� �� enderTargetView.
		// �޸𸮸� ��(��) �ϳ��� ����

		// @Test.
		mesh->Update(1.0f / 60.0f);
		mesh2->Update(1.0f / 60.0f);

		// ��ο�(Draw) (Draw).		��ο� �� �� ������ ����������
		mesh->Draw();
		mesh2->Draw();
		//mesh3->Draw();

		/*
		// ���ҽ� ���ε�.
		// ���� ���� ����.
		static unsigned int stride = Vector3::Stride();
		static unsigned int offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		// �ε��� ���� ����.
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		
		//// �Է� ���̾ƿ� ����.
		//context->IASetInputLayout(inputlayout);

		//// ������ ��� ����.
		//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// ���̴� ����.
		//context->VSSetShader(vertexShader, nullptr, 0);
		//context->PSSetShader(pixelShader, nullptr, 0);

		shader->Bind();

		// ��ο� ��.
		context->DrawIndexed(3, 0, 0);

		*/

		// ���� ��ȯ (EndScene/Present).
		swapChain->Present(1u, 0u);		// 1: ���ڴ� / 0: �� ���ڴ�
	}
}