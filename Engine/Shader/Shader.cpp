#include "Shader.h"
#include <d3dcompiler.h>
#include "../Core/Engine.h"

namespace Blue
{
	Shader::Shader(const std::wstring& name)
		: name(name)
	{
		// 경로 추가.
		wchar_t path[256] = { };
		swprintf_s(path, 256, L"HLSLShader/%sVertexShader.hlsl", name.c_str());		// %s: name

		// 정점 쉐이더 컴파일(해석).
		//ID3DBlob* vertexShaderBuffer = nullptr;		// 쉐이더 정보 저장용
		auto result = D3DCompileFromFile(
			//TEXT("VertexShader.hlsl"),
			path,
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

		// 장치 객체 얻어오기.
		ID3D11Device& device = Engine::Get().Device();

		// 정점 쉐이더 생성.
		result = device.CreateVertexShader(
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
		// 정점 쉐이더에 전달할 정점 데이터가 어떻게 생겼는지 알려줌, 설계도. (쉐이더에서 만드는 데이터와 넘겨주는 정점 데이터가 같아야 함?)
		/*
		//LPCSTR SemanticName;
		//UINT SemanticIndex;
		//DXGI_FORMAT Format;
		//UINT InputSlot;
		//UINT AlignedByteOffset;
		//D3D10_INPUT_CLASSIFICATION InputSlotClass;
		//UINT InstanceDataStepRate;
		*/
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	// float: 32bit (4차원 벡터는 뒤에 a도 붙음)
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		result = device.CreateInputLayout(
			inputDesc,
			_countof(inputDesc),
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
		// 각 리소스 바인딩. - x?
		// 픽셀 쉐이더 컴파일.
		//ID3DBlob* pixelShaderBuffer = nullptr;
		swprintf_s(path, 256, L"HLSLShader/%sPixelShader.hlsl", name.c_str());		// %s: name
		result = D3DCompileFromFile(
			path,
			nullptr,
			nullptr,
			"main",
			"ps_5_0",
			0, 0,
			&pixelShaderBuffer, nullptr
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to compile pixel shader.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 픽셀 쉐이더 생성.
		result = device.CreatePixelShader(
			pixelShaderBuffer->GetBufferPointer(),
			pixelShaderBuffer->GetBufferSize(),
			nullptr,
			&pixelShader
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create pixel shader.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}
	}

	Shader::~Shader()
	{
		// DX 리소스 해제.
		if (inputlayout)
		{
			inputlayout->Release();
		}

		if (vertexShader)
		{
			vertexShader->Release();
		}

		if (vertexShaderBuffer)
		{
			vertexShaderBuffer->Release();
		}

		if (pixelShader)
		{
			pixelShader->Release();
		}

		if (pixelShaderBuffer)
		{
			pixelShaderBuffer->Release();
		}
	}

	void Shader::Bind()
	{
		// 장치 문맥(DeviceContext) 얻어오기.
		// static이므로 한 번만 실행, static이므로 프로그램 종료 시 자동 소멸
		static ID3D11DeviceContext& context = Engine::Get().Context();

		// 입력 레이아웃 전달.
		context.IASetInputLayout(inputlayout);

		// 조립할 모양 설정.
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 쉐이더 설정.
		context.VSSetShader(vertexShader, nullptr, 0);
		context.PSSetShader(pixelShader, nullptr, 0);
	}
}