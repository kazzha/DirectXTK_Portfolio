#include "pch.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
	JsonManager::Instance().ReleaseAll();
	TextureManager::Instance().ReleaseAll();
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

	TextureManager::Instance().Initialize(m_deviceResources.get());
	m_Object.resize(56, nullptr);
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
	auto kb = Keyboard::Get().GetState();

	if (kb.Escape)
	{
		ExitGame();
	}

	ActorManager::Instance().Update(timer.GetElapsedSeconds());
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

void Game::processInput(WPARAM wParam)
{
	m_Player->Update(m_timer.GetElapsedSeconds(),wParam);
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

	ActorManager().Instance().Draw(m_spriteBatch.get());

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

	
	
}

void Game::CreateWindowSizeDependentResources()
{

}

void Game::OnDeviceLost()
{
	m_spriteBatch.reset();
	m_commonStates.reset();

	TextureManager::Instance().OnDeviceLost();
	ActorManager::Instance().OnDeviceLost();
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	TextureManager::Instance().OnDeviceRestored();
	ActorManager::Instance().OnDeviceRestored();
}
#pragma endregion