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
	if (m_options & c_EnableHDR) // 해당 플래그가 켜져 있는지 확인
	{
		ComPtr<IDXGIFactory5> factory5;
		if (FAILED(m_dxgiFactory.As(&factory5)))
		{
			m_options &= ~c_EnableHDR;
#ifdef _DEBUG
			OutputDebugStringA("WARNING: HDR swap chains not supported");
#endif
		}
	}
	if (m_options & c_FlipPresent)
	{
		ComPtr<IDXGIFactory4> factory4;
		if (FAILED(m_dxgiFactory.As(&factory4)))
		{
			m_options &= ~c_FlipPresent;
#ifdef _DEBUG
			OutputDebugStringA("INFO: Flip swap effects not supported");
#endif
		}
	}
	static const D3D_FEATURE_LEVEL s_featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	UINT featLevelCount = 0;
	for (; featLevelCount < _countof(s_featureLevels); ++featLevelCount)
	{
		if (s_featureLevels[featLevelCount] < m_d3dMinFeatureLevel)
			break;
	}
	if (!featLevelCount)
	{
		throw std::out_of_range("minFeatureLevel too high");
	}
	ComPtr<IDXGIAdapter1> adapter; 
	GetHardwareAdapter(adapter.GetAddressOf()); 

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = E_FAIL;
	if (adapter)
	{
		hr = D3D11CreateDevice(
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			s_featureLevels,
			featLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&m_d3dFeatureLevel,
			context.GetAddressOf()
		);
	}
#if defined(NDEBUG)
	else
	{
		throw std::exception("No Direct3D hardware device found");
	}
#else
	if (FAILED(hr))
	{
		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
			nullptr,
			creationFlags,
			s_featureLevels,
			featLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&m_d3dFeatureLevel,
			context.GetAddressOf()
		);

		if (SUCCEEDED(hr))
		{
			OutputDebugStringA("Direct3D Adapter - WARP\n");
		}
	}
#endif
	ThrowIfFailed(hr);

#ifdef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[]=
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif
	ThrowIfFailed(device.As(&m_d3dDevice));
	ThrowIfFailed(context.As(&m_d3dContext));
	ThrowIfFailed(context.As(&m_d3dAnnotaion));
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
