#pragma once

#include "Core/Type.h"
#include <string>			// �׸� ������ �о�;� �ϹǷ�
#include <d3d11.h>
#include <memory>

namespace Blue
{
	// �ؽ�ó ������ �о ������ ����ü.
	struct TextureData
	{
		TextureData() = default;
		~TextureData()
		{
			// ���� ������ ����.
			free(data);
			data = nullptr;

			// DX ���ҽ� ����.
			shaderResourceView->Release();
			shaderResourceView = nullptr;
		}

		// �ؽ�ó ���� ������.
		int32 width = 0;
		int32 height = 0;
		int32 channelCount = 0;
		void* data = nullptr;	// ���� �Ҵ��ؼ� ���� �ִ� �ּҸ� �Ѱ���

		// DX ���ҽ�.
		ID3D11ShaderResourceView* shaderResourceView = nullptr;		// DX �������δ� �갡 ���̴�
		ID3D11SamplerState* samplerState = nullptr;
	};

	// �ؽ�ó Ŭ����.
	class Texture
	{
		// ���ε��� ���̴� ������.
		enum class BindType
		{
			VertexShader,
			PixelShader
		};

	public:
		Texture();
		Texture(const std::string& name,
			BindType bindType = BindType::PixelShader,
			uint32 index = 0u);
		~Texture();

		void Bind();

	private:
		void LoadTexture(const std::string& name);

	private:
		// �̹��� �̸�.
		std::string name;
		
		// �ؽ�ó ����.
		uint32 index = 0u;

		// ���ε� ���̴� Ÿ��. (��� ���ε� �Ұ���)
		BindType bindType = BindType::PixelShader;

		// �ؽ�ó ������.
		std::unique_ptr<TextureData> textureData;
	};
}