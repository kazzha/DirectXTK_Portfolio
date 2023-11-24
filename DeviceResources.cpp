#include "pch.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DX;

using Microsoft::WRL::ComPtr;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

namespace
{
#if defined(_DEBUG)
	inline bool SdkLayersAvailable() noexcept
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_NULL, nullptr,
			D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION,
			nullptr,
			nullptr,
			nullptr
		);
		return SUCCEEDED(hr);
	}
#endif
	inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt) noexcept
	{
		switch (fmt)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8X8_UNORM;
		default:
			return fmt;
		}
	}
}

DeviceResources::DeviceResources(DXGI_FORMAT backBufferFormat,
	DXGI_FORMAT depthBufferFormat,
	UINT backBufferCount,
	D3D_FEATURE_LEVEL minFeatureLevel,
	unsigned int flags) noexcept :
	m_screenViewport{},
	m_backBufferFormat(backBufferFormat),
	m_depthBufferFormat(depthBufferFormat),
	m_backBufferCount(backBufferCount),
	m_d3dMinFeatureLevel(minFeatureLevel),
	m_window(nullptr),
	m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1),
	m_outputSize{0,0,1,1},
	m_colorSpace(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709),
	m_options(flags | c_FlipPresent),
	m_deviceNotify(nullptr)
{
}

void DeviceResources::CreateDeviceResources()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (SdkLayersAvailable())
	{
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
	else
	{
		OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
	}
#endif
	CreateFactory();

	if (m_options & c_AllowTearing)
	{
		BOOL allowTearing = FALSE;

		ComPtr<IDXGIFactory5> factory5;
		HRESULT hr = m_dxgiFactory.As(&factory5);
		if (SUCCEEDED(hr))
		{
			hr = factory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing,
				sizeof(allowTearing)
			);
		}

		if (FAILED(hr) || !allowTearing)
		{
			m_options &= ~c_AllowTearing;
#ifdef _DEBUG
			OutputDebugStringA(
				"WARNING: Variable refresh rate displays not supported"
			);
#endif
		}
	}
}

void DeviceResources::CreateWindowSizeDependentResources()
{
}

void DeviceResources::SetWindow(HWND window, int width, int height) noexcept
{
}

bool DeviceResources::WindowSizeChanged(int width, int height)
{
	return false;
}

void DeviceResources::HandleDeviceLost()
{
}

void DeviceResources::Present()
{
}
