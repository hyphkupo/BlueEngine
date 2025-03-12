#include "Renderer.h"
#include <vector>
#include <d3dcompiler.h>	// 쉐이더 처리를 위해 추가

#include "../Math/Vector3.h"
//#include "../Shader/Shader.h"
#include "TriangleMesh.h"
#include "QuadMesh.h"
#include "Core/Common.h"

namespace Blue
{
	Renderer::Renderer(uint32 width, uint32 height, HWND window)
	{
		// 장치 생성에 사용하는 옵션.
		uint32 flag = 0u;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// 생성할 라이브러리 버전.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,		// 그래픽카드가 이 버전 지원 안 하면 다음 거 사용 , ... 반복
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL outFeatureLevel;

		// 장치 생성.
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

		//스왑 체인 정보 구조체.
		//DXGI_MODE_DESC BufferDesc;
		//DXGI_SAMPLE_DESC SampleDesc;
		//DXGI_USAGE BufferUsage;
		//UINT BufferCount;
		//HWND OutputWindow;
		//BOOL Windowed;
		//DXGI_SWAP_EFFECT SwapEffect;
		//UINT Flags;

		// 멀티 샘플링 지원 여부 확인.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// 창 모드?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;		// 백버퍼 개수.	=> 일반적으론 개수가 늘어날수록 창 바꿀 때 부드러움
		swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 개수. (멀티 샘플링 개수 1개 => 멀티 샘플링을 안 하겠다, 안 쓰겠다는 뜻)
		swapChainDesc.SampleDesc.Quality = 0;	// 멀티 샘플링 수준. (샘플링 값은 보통 count - 1)
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 8bit 4개이므로 32bit, unsigned(부호없음), normalize(정규화됨 => 0~1까지의 값으로 매핑시키겠다)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// D3D_FEATURE_LEVEL targetLevel;

		// 장치 생성.
		//ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		//	nullptr,
		//	D3D_DRIVER_TYPE_HARDWARE,
		//	nullptr,
		//	flag,
		//	featureLevels,
		//	_countof(featureLevels),	// 배열의 개수
		//	D3D11_SDK_VERSION,
		//	&swapChainDesc,
		//	&swapChain,	// 버퍼 교환
		//	&device,	// 리소스 생성 (CPU)
		//	nullptr,	// nullptr 대신 D3D_FEATURE_LEVEL 변수 만들어서 넣으면 버전 맞는지 확인...?
		//	&context	// 바인딩 (GPU로 넘김) => 연결, DRAW
		//	// dx는 보통 set이라는 이름을 가짐 (설정, 연결, 바인딩)
		//), TEXT("Failed to create devices."));

		// 장치 생성.
		ThrowIfFailed(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),	// 배열의 개수
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&context
		), TEXT("Failed to create devices."));

		// IDXGIFactory 리소스 생성.
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
		
		// 결과 확인.
		//if (FAILED(result))		// result < 0과 같음
		//{
		//	MessageBoxA(nullptr, "Failed to create devices.", "Error", MB_OK);		// ~A: 아스키 버전
		//	__debugbreak();
		//}

		// SwapChain 생성.
		ThrowIfFailed(factory->CreateSwapChain(
			device,
			&swapChainDesc,
			&swapChain
		), TEXT("Failed to create a swap chain."));

		// 렌더 타겟 뷰 생성.		// 뷰 => 리소스 => 리소스는 덩어리 (포인터 생각!)
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

		// 렌더 타겟 뷰 바인딩(연결).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);		// OutputMerger (=> 최종 출력 단계에 사용할 함수다) => render target 넘김

		// 뷰포트(화면).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// 뷰포트 설정.
		context->RSSetViewports(1, &viewport);		// rs(래스터라이저) 단계에서 사용한다

		// 정점 데이터 생성.
		//std::vector
		// vertex -> vertices.
		Vector3 vertices[] =
		{
			Vector3(0.0f, 0.5f, 0.5f),
			Vector3(0.5f, -0.5f, 0.5f),
			Vector3(-0.5f, -0.5f, 0.5f),
		};

		/*
		//// @Temp: 임시 리소스 생성.
		//// 버퍼(Buffer) - 메모리 덩어리.
		//D3D11_BUFFER_DESC vertexbufferDesc = {};
		//vertexbufferDesc.ByteWidth = Vector3::Stride() * 3;
		//vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//// 정점 데이터.
		//D3D11_SUBRESOURCE_DATA vertexData = {};
		//// D3D11_SUBRESOURCE_DATA: 여러 데이터를 버퍼 하나에 몰아넣을 때 사용
		//vertexData.pSysMem = vertices;

		//// (정점)버퍼 생성
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

		//// 인덱스(색인) 버퍼 생성(정점을 조립하는 순서).
		//int indices[] = { 0, 1, 2 };	// 시계 방향(왼손 좌표계)
		//// 방향을 확인할 때 cos 사용 => 왜?

		//// @Temp: 임시 리소스 생성.
		//// 버퍼(Buffer) - 메모리 덩어리.
		//D3D11_BUFFER_DESC indexbufferDesc = {};
		//indexbufferDesc.ByteWidth = sizeof(int) * 3;
		//indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 버퍼의 사용 목적도 적어줘야 함

		//// 정점 데이터.
		//D3D11_SUBRESOURCE_DATA indexData = {};		// 실제 데이터
		//// D3D11_SUBRESOURCE_DATA: 여러 데이터를 버퍼 하나에 몰아넣을 때 사용
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

		//// 정점 쉐이더 컴파일(해석).
		//ID3DBlob* vertexShaderBuffer = nullptr;		// 쉐이더 정보 저장용
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

		//// 정점 쉐이더 생성.
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

		//// 입력 레이아웃.
		//// 정점 쉐이더에 전달할 정점 데이터가 어떻게 생겼는지 알려줌, 설계도. (쉐이더에서 만드는 데이터와 넘겨주는 정점 데이터가 같아야 함?)
		////LPCSTR SemanticName;
		////UINT SemanticIndex;
		////DXGI_FORMAT Format;
		////UINT InputSlot;
		////UINT AlignedByteOffset;
		////D3D10_INPUT_CLASSIFICATION InputSlotClass;
		////UINT InstanceDataStepRate;
		//D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		//{
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	// float: 32bit (4차원 벡터는 뒤에 a도 붙음)
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

		//// 픽셀 쉐이더 컴파일/생성.
		//// 각 리소스 바인딩. - x?
		//// 픽셀 쉐이더 컴파일.
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

		//// 픽셀 쉐이더 생성.
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

		// 쉐이더 객체 생성.
		//shader = std::make_unique<Shader>();
		*/
	}

	Renderer::~Renderer()
	{
		device->Release();				
		context->Release();		
		swapChain->Release();

		// 버퍼.
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

		// 그리기 전 작업 (BeginScene).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);

		// 지우기(Clear).			지우는 작업은 사실 없음 ... 색상 하나로 덮는 것
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);	// 우리가 이미지로 사용하는 건 enderTargetView.
		// 메모리를 값(색) 하나로 덮음

		// @Test.
		mesh->Update(1.0f / 60.0f);
		mesh2->Update(1.0f / 60.0f);

		// 드로우(Draw) (Draw).		드로우 콜 시 렌더링 파이프라인
		mesh->Draw();
		mesh2->Draw();
		//mesh3->Draw();

		/*
		// 리소스 바인딩.
		// 정점 버퍼 전달.
		static unsigned int stride = Vector3::Stride();
		static unsigned int offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		// 인덱스 버퍼 전달.
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		
		//// 입력 레이아웃 전달.
		//context->IASetInputLayout(inputlayout);

		//// 조립할 모양 설정.
		//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// 쉐이더 설정.
		//context->VSSetShader(vertexShader, nullptr, 0);
		//context->PSSetShader(pixelShader, nullptr, 0);

		shader->Bind();

		// 드로우 콜.
		context->DrawIndexed(3, 0, 0);

		*/

		// 버퍼 교환 (EndScene/Present).
		swapChain->Present(1u, 0u);		// 1: 쓰겠다 / 0: 안 쓰겠다
	}
}