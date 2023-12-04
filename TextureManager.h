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
	};
}


