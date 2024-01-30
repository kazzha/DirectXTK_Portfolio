#include "pch.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;

using Microsoft::WRL::ComPtr;

enum class Layer : int
{
	Background = 0,
	Tile = 1,
	Goal = 2,
	Box = 3,
	Character = 4,
	Wall = 5
};



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
	m_Stage1.resize(56, nullptr);

	Stage1Initialize();

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

void Game::Stage1Initialize()
{

	m_Player = ActorManager::Instance().Create<Player>(static_cast<int>(Layer::Character),
		L"Assets/playerSprite.png", L"Assets/playerSprite.json", L"Assets/PlayerAnimation.json");

	m_Player->SetAnimation(L"MoveRight");
	m_Player->SetPosition(224.0f, 160.0f);
	m_Stage1[9] = m_Player;
	m_Player->Stop();
	m_Player->StageInfo = &m_Stage1;
	m_Player->PlayerIndex = 9;

	float startXposition{ 160.0f };
	float startYposition{ 96.0f };

	for (int i = 0; i < 56; i++)
	{
		if (i != 2 && i != 39 && i != 47 && i < 53)
		{
			if (startYposition == 96.0f || startYposition == 480.0f || startXposition == 160.0f || startXposition == 608.0f) {
				m_Stage1[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/red_block.png");
				m_Stage1[i]->SetPosition(startXposition, startYposition);
			}
		}
		else
		{
			m_Stage1[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
				L"Assets/brown_block.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
			L"Assets/ground_06.png");
		pTile->SetPosition(startXposition, startYposition);
		if (i == 11 || i == 43)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
				L"Assets/red_block.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 18 || i == 21 || i == 26)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/brown_box.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 28)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/blue_box.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 34)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/red_snowbox.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 37)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/green_box.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 19)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_red.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 20 || i == 27)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_brown.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 35)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_green.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}
		else if (i == 36)
		{
			m_Stage1[i] = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_blue.png");
			m_Stage1[i]->SetPosition(startXposition, startYposition);
		}


		startXposition += 64.0f;
		if (i % 8 == 7) { startYposition += 64.0f; startXposition = 160.0f; }
	}

	remainBox = 3;
	walkCount = 0;
}

void Game::Stage2Initialize()
{
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

	std::wstring output = L"Walk Count: " + std::to_wstring(walkCount) + L"  /  Remain Box Count: " + std::to_wstring(remainBox);

	m_fontPos.x = 125.f;
	m_fontPos.y = 20.f;

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());


	m_font->DrawString(m_spriteBatch.get(), output.c_str(),
		m_fontPos, Colors::White, 0.f);

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

	context->ClearRenderTargetView(renderTarget, Colors::DimGray);
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

	m_font = std::make_unique<SpriteFont>(device, L"Assets/dung.spritefont");
	
}

void Game::CreateWindowSizeDependentResources()
{
	
}

void Game::OnDeviceLost()
{
	m_spriteBatch.reset();
	m_commonStates.reset();
	m_font.reset();

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