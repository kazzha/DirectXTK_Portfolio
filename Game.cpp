#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{

}

void Game::Initialize(HWND window, int width, int height)
{
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window); // 마우스를 어떤 윈도우에서 사용할지 마우스 내부에 window 저장

	m_deviceResources->SetWindow(window, width, height); 

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma region Frame Update

void Game::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

void Game::Update(DX::StepTimer const& timer)
{
	auto kb = m_keyboard->GetState();
	if (kb.Escape)
	{
		ExitGame();
	}
}

#pragma endregion




#pragma region Message Handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();
}

void Game::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();

	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
}

void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	width = 800;
	height = 600;
}
#pragma endregion

#pragma region Frame Render

void Game::Render()
{
	if (m_timer.GetFrameCount() == 0) // 한번도 Update()가 호출된 적이 없으면 렌더링 하지 않음
	{
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	m_spriteBatch->Draw(
		m_texBug.Get(),
		XMFLOAT2(0.0f, 0.0f),
		nullptr,
		Colors::White,
		0.0f,
		XMFLOAT2(0.0f, 0.0f),
		3.0f
	);
	m_spriteBatch->Draw(
		m_texCat.Get(), 
		XMFLOAT2(50.0f, 0.0f),
		nullptr,
		Colors::White,
		0.0f,
		XMFLOAT2(0.0f, 0.0f));
	m_spriteBatch->End();

	m_deviceResources->PIXEndEvent();
	
	m_deviceResources->Present();
}

void Game::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(
		depthStencil,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

#pragma endregion

#pragma region Direct3D Resources
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_commonStates = std::make_unique<CommonStates>(device);

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Assets/bug1_1.png", nullptr,
			m_texBug.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Assets/bapburger.png", nullptr,
			m_texCat.ReleaseAndGetAddressOf()
		)
	);

}

void Game::CreateWindowSizeDependentResources()
{

}

void Game::OnDeviceLost()
{
	m_texCat.Reset();
	m_texBug.Reset();
	m_spriteBatch.reset();
	m_commonStates.reset();
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
#pragma endregion