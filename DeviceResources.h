#pragma once


namespace DX
{
	class IDeviceNotify // interface
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		~IDeviceNotify() = default;

	};
	class DeviceResources
	{
	public:
		static const unsigned int c_FlipPresent = 0x1;
		static const unsigned int c_AllowTearing = 0x2;
		static const unsigned int c_EnableHDR = 0x4;

		DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
			DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
			UINT backBufferCount = 2,
			D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_10_0,
			unsigned int flags = c_FlipPresent) noexcept;

		~DeviceResources() = default;
		
		DeviceResources(DeviceResources&&) = default; // 이동 생성자
		DeviceResources& operator= (DeviceResources&&) = default; // 이동 대입 연산자

		DeviceResources(DeviceResources const&) = delete;
		DeviceResources& operator= (DeviceResources const&) = delete;

		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void SetWindow(HWND window, int width, int height) noexcept;
		bool WindowSizeChanged(int width, int height);
		void HandleDeviceLost();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify) noexcept
		{
			m_deviceNotify = deviceNotify;
		}
		void Present();

	    RECT GetOutputSize() const noexcept
		{
			return m_outputSize;
		}
		auto GetD3DDevice() const noexcept
		{
			return m_d3dDevice.Get();
		}
		auto GetD3DDeviceContext() const noexcept
		{
			return m_d3dContext.Get();
		}
		auto GetSwapChain() const noexcept
		{
			return m_swapChain.Get();
		}
		auto GetDXGIFactory() const noexcept
		{
			return m_dxgiFactory.Get();
		}
		HWND GetWindow() const noexcept
		{
			return m_window;
		}
		D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const noexcept
		{
			return m_d3dFeatureLevel;
		}
		ID3D11Texture2D* GetRenderTarget() const noexcept
		{
			return m_renderTarget.Get();
		}
		ID3D11Texture2D* GetDepthStencil() const noexcept
		{
			return m_depthStencil.Get();
		}
		ID3D11RenderTargetView* GetRenderTargetView() const noexcept
		{
			return m_d3dRenderTargetView.Get();
		}
		ID3D11DepthStencilView* GetDepthStencilView() const noexcept
		{
			return m_d3dDepthStencilView.Get();
		}
		DXGI_FORMAT GetBackBufferFormat() const noexcept
		{
			return m_backBufferFormat;
		}
		DXGI_FORMAT GetDepthBufferFormat() const noexcept
		{
			return m_depthBufferFormat;
		}
		D3D11_VIEWPORT GetScreenViewport() const noexcept
		{
			return m_screenViewport;
		}
		UINT GetBackBufferCount() const noexcept
		{
			return m_backBufferCount;
		}
		DXGI_COLOR_SPACE_TYPE GetColorSpace() const noexcept
		{
			return m_colorSpace;
		}
		unsigned int GetDeviceOption() const noexcept
		{
			return m_options;
		}
		void PIXBeginEvent(_In_z_ const wchar_t* name)
		{
			m_d3dAnnotaion->BeginEvent(name);
		}
		void PIXEndEvent()
		{
			m_d3dAnnotaion->EndEvent();
		}
		void PIXSetMarker(_In_z_ const wchar_t* name)
		{
			m_d3dAnnotaion->SetMarker(name);
		}

	private:
		void CreateFactory();
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
		void UpdateColorSpace();

		Microsoft::WRL::ComPtr<IDXGIFactory2> m_dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> m_d3dAnnotaion;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTarget;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3dDepthStencilView;
		D3D11_VIEWPORT m_screenViewport;

		DXGI_FORMAT m_backBufferFormat;
		DXGI_FORMAT m_depthBufferFormat;
		UINT m_backBufferCount;
		D3D_FEATURE_LEVEL m_d3dMinFeatureLevel;

		HWND m_window;
		D3D_FEATURE_LEVEL m_d3dFeatureLevel;
		RECT m_outputSize;

		DXGI_COLOR_SPACE_TYPE m_colorSpace;

		unsigned int m_options;

		IDeviceNotify* m_deviceNotify;

	
	};
}