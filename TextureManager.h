#pragma once

namespace DX
{
	class TextureManager final : public IDeviceNotify
	{
	public:
		static TextureManager& Instance()
		{
			static TextureManager instance;
			return instance;
		}

	public:
		TextureManager();
		~TextureManager();

		TextureManager(TextureManager&&) = delete;
		TextureManager& operator= (TextureManager&&) = delete;

		TextureManager(TextureManager const&) = delete;
		TextureManager& operator= (TextureManager const&) = delete;

	public:
		void OnDeviceLost() override;
		void OnDeviceRestored() override;
		void Initialize(DeviceResources* pDeviceResources);
		void ReleaseAll();

		ID3D11ShaderResourceView* CreateShaderResourceView(LPCWSTR filename);
		void Remove(LPCWSTR filename);

	private:
		DeviceResources* m_pDeviceResources;
		std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_shaderResources;
		// ID3D11ShaderResourceView : GPU에 저장된 텍스쳐를 세이더에서 읽을 수 있도록 하는 뷰 -> 세이더에서 텍스쳐의 픽셀 데이터를 읽을 수 있게 됨
		// 세이더는 바보라서 텍스쳐 샘플러라는 텍스처를 읽을 도구를 가지고 텍스처를 읽음.
	};
}


