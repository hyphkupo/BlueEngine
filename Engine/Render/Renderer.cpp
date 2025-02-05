#include "Renderer.h"
#include <vector>
#include <d3dcompiler.h>	// 쉐이더 처리를 위해 추가

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
		swapChainDesc.BufferCount = 1;		// 백버퍼 개수.	=> 일반적으론 개수가 늘어날수록 창 바꿀 때 부드러움
		swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 개수. (멀티 샘플링 개수 1개 => 멀티 샘플링을 안 하겠다, 안 쓰겠다는 뜻)
		swapChainDesc.SampleDesc.Quality = 0;	// 멀티 샘플링 수준. (샘플링 값은 보통 count - 1)
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 8bit 4개이므로 32bit, unsigned(부호없음), normalize(정규화됨 => 0~1까지의 값으로 매핑시키겠다)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// D3D_FEATURE_LEVEL targetLevel;

		// 장치 생성.
		HRESULT result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain,
			&device,
			nullptr,	// nullptr 대신 D3D_FEATURE_LEVEL 변수 만들어서 넣으면 버전 맞는지 확인...?
			&context
		);

		// 결과 확인.
		if (FAILED(result))		// result < 0과 같음
		{
			MessageBoxA(nullptr, "Failed to create devices.", "Error", MB_OK);		// ~A: 아스키 버전
			__debugbreak();
		}

		// 렌더 타겟 뷰 생성.
		ID3D11Texture2D* backbuffer = nullptr;
		//swapChain->GetBuffer(
		//	0, 
		//	__uuidof(backbuffer), 
		//	reinterpret_cast<void**>(&backbuffer)
		//);
		result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to get back buffer.", "Error", MB_OK);		// ~A: 아스키 버전
			__debugbreak();
		}

		result = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to create render targer view.", "Error", MB_OK);		// ~A: 아스키 버전
			__debugbreak();
		}

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
		float vertices[] =
		{
			0.0f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
		};

		// @Temp: 임시 리소스 생성.
		// 버퍼(Buffer) - 메모리 덩어리.
		D3D11_BUFFER_DESC vertexbufferDesc = {};
		vertexbufferDesc.ByteWidth = sizeof(float) * 3 * 3;
		vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// 정점 데이터.
		D3D11_SUBRESOURCE_DATA vertexData = {};
		// D3D11_SUBRESOURCE_DATA: 여러 데이터를 버퍼 하나에 몰아넣을 때 사용
		vertexData.pSysMem = vertices;

		// (정점)버퍼 생성
		device->CreateBuffer(
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
		int indices[] = { 0, 1, 2 };	// 시계 방향(왼손 좌표계)

		// @Temp: 임시 리소스 생성.
		// 버퍼(Buffer) - 메모리 덩어리.
		D3D11_BUFFER_DESC indexbufferDesc = {};
		indexbufferDesc.ByteWidth = sizeof(int) * 3;
		indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// 정점 데이터.
		D3D11_SUBRESOURCE_DATA indexData = {};
		// D3D11_SUBRESOURCE_DATA: 여러 데이터를 버퍼 하나에 몰아넣을 때 사용
		indexData.pSysMem = indices;

		result = device->CreateBuffer(
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

		// 쉐이더 컴파일.
		ID3DBlob* vertexShaderBuffer = nullptr;		// 쉐이더 정보 저장용
		result = D3DCompileFromFile(
			TEXT("VertexShader.hlsl"),
			nullptr,
			nullptr,
			"main",		// pEntrypoint
			"vs_5_0",	// pTarget
			0, 0,
			&vertexShaderBuffer, nullptr
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to compile vertex shader.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 쉐이더 생성.
		result = device->CreateVertexShader(
			vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(),
			nullptr,
			&vertexShader
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create vertex shader.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 입력 레이아웃.
		// 정점 쉐이더에 전달할 정점 데이터가 어떻게 생겼는지 알려줌. (쉐이더에서 만드는 데이터와 넘겨주는 정점 데이터가 같아야 함?)
		//LPCSTR SemanticName;
		//UINT SemanticIndex;
		//DXGI_FORMAT Format;
		//UINT InputSlot;
		//UINT AlignedByteOffset;
		//D3D10_INPUT_CLASSIFICATION InputSlotClass;
		//UINT InstanceDataStepRate;
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	// float: 32bit (4차원 벡터는 뒤에 a도 붙음)
			  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		result = device->CreateInputLayout(
			inputDesc,
			1,
			vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(),
			&inputlayout
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create input layout.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 픽셀 쉐이더 컴파일/생성.
		// 각 리소스 바인딩.
	}

	Renderer::~Renderer()
	{
	}
	
	void Renderer::Draw()
	{
		// 그리기 전 작업 (BeginScene).
		// 지우기(Clear).
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);	// 우리가 이미지로 사용하는 건 enderTargetView.
		// 메모리를 값 하나로 덮음

		// 드로우(Draw) (Draw).

		// 버퍼 교환 (EndScene/Present).
		swapChain->Present(1u, 0u);		// 1: 쓰겠다 / 0: 안 쓰겠다
	}
}