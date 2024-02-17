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
	m_Player = nullptr;
	walkCount = 0;
	remainBox = 0;
	stageNum = 1;
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

	if (remainBox == 0)
	{
		stageNum = 2;
		StageChange();
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

void Game::processInput(WPARAM wParam)
{
	m_Player->Update(m_timer.GetElapsedSeconds(),wParam);
}

void Game::Stage1Initialize()
{
	m_Stage1.resize(56, nullptr);
	m_Player = ActorManager::Instance().Create<Player>(static_cast<int>(Layer::Character),
		L"Assets/playerSprite.png", L"Assets/playerSprite.json", L"Assets/PlayerAnimation.json");

	m_Player->SetAnimation(L"MoveRight");
	m_Player->SetPosition(224.0f, 160.0f);
	m_Stage1[9] = m_Player;
	m_Player->Stop();
	m_Player->StageInfo = &m_Stage1;
	m_Player->PlayerIndex = 9;
	m_Player->walkCount = &walkCount;
	m_Player->indexRow = 8;

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
			Wall* wall = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
				L"Assets/red_block.png");
			wall->SetPosition(startXposition, startYposition);
			m_Stage1[i] = wall;
		}
		else if (i == 18 || i == 21 || i == 26)
		{
			Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/brown_box.png");
			box->SetPosition(startXposition, startYposition);
			box->StageInfo = &m_Stage1;
			box->remainBoxCount = &remainBox;
			box->boxIndex = i;
			box->color = "brown";
			box->indexRow = 8;
			m_Stage1[i] = box;
			
		}
		else if (i == 28)
		{
			Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/blue_box.png");
			box->SetPosition(startXposition, startYposition);
			box->StageInfo = &m_Stage1;
			box->remainBoxCount = &remainBox;
			box->boxIndex = i;
			box->color = "blue";
			box->indexRow = 8;
			m_Stage1[i] = box;
		}
		else if (i == 34)
		{
			Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/red_snowbox.png");
			box->SetPosition(startXposition, startYposition);
			box->StageInfo = &m_Stage1;
			box->remainBoxCount = &remainBox;
			box->boxIndex = i;
			box->color = "red";
			box->indexRow = 8;
			m_Stage1[i] = box;
		}
		else if (i == 37)
		{
			Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
				L"Assets/green_box.png");
			box->SetPosition(startXposition, startYposition);
			box->StageInfo = &m_Stage1;
			box->remainBoxCount = &remainBox;
			box->boxIndex = i;
			box->color = "green";
			box->indexRow = 8;
			m_Stage1[i] = box;
		}
		else if (i == 19)
		{
			Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_red.png");
			goal->SetPosition(startXposition, startYposition);
			goal->color = "red";
			m_Stage1[i] = goal;
		}
		else if (i == 20 || i == 27)
		{
			Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_brown.png");
			goal->SetPosition(startXposition, startYposition);
			goal->color = "brown";
			m_Stage1[i] = goal;
		}
		else if (i == 35)
		{
			Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_green.png");
			goal->SetPosition(startXposition, startYposition);
			goal->color = "green";
			m_Stage1[i] = goal;
		}
		else if (i == 36)
		{
			Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
				L"Assets/dia_blue.png");
			goal->SetPosition(startXposition, startYposition);
			goal->color = "blue";
			m_Stage1[i] = goal;
		}

		startXposition += 64.0f;
		if (i % 8 == 7) { startYposition += 64.0f; startXposition = 160.0f; }
	}

	remainBox = 5;
	walkCount = 0;
}

void Game::Stage2Initialize()
{
	m_Stage2.resize(70, nullptr);
	m_Player = ActorManager::Instance().Create<Player>(static_cast<int>(Layer::Character),
		L"Assets/playerSprite.png", L"Assets/playerSprite.json", L"Assets/PlayerAnimation.json");

	m_Player->SetAnimation(L"MoveForward");
	m_Player->SetPosition(440.0f, 160.0f);
	m_Stage2[15] = m_Player;
	m_Player->Stop();
	m_Player->StageInfo = &m_Stage2;
	m_Player->PlayerIndex = 15;
	m_Player->walkCount = &walkCount;
	m_Player->indexRow = 10;

	walkCount = 0;
	remainBox = 4;

	float startXposition{ 120.0f };
	float startYposition{ 96.0f };

	for (int i = 0; i < 70; i++)
	{
		if (i / 10 == 0)
		{
			auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
				L"Assets/ground_05.png");
			pTile->SetPosition(startXposition, startYposition);
			
			if (i == 4 || i == 5 || i == 6)
			{
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			}
		}
		else if (i / 10 == 1)
		{
			if (i != 15)
			{
				auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
					L"Assets/ground_05.png");
				pTile->SetPosition(startXposition, startYposition);
			}
			else
			{
				auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
					L"Assets/ground_06.png");
				pTile->SetPosition(startXposition, startYposition);
			}
			if (i == 12 || i == 13 || i == 14 || i == 16 || i == 17 || i == 18)
			{
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			}
			
		}
		else if (i / 10 == 2)
		{
			auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
				L"Assets/ground_06.png");
			pTile->SetPosition(startXposition, startYposition);
			
			 if (i == 20 || i == 21 || i == 22 || i == 28 || i == 29)
			 { 
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			 }
			 if (i == 25)
			 {
				 Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
					 L"Assets/dia_blue.png");
				 goal->SetPosition(startXposition, startYposition);
				 goal->color = "blue";

				 Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
					 L"Assets/green_box.png");
				 box->SetPosition(startXposition, startYposition);
				 box->StageInfo = &m_Stage2;
				 box->remainBoxCount = &remainBox;
				 box->boxIndex = i;
				 box->color = "green";
				 box->indexRow = 10;
				 box->alreadyGoal = goal;
				 m_Stage2[i] = box;
			 }
			 if (i == 26)
			 { 
				 Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
					 L"Assets/dia_blue.png");
				 goal->SetPosition(startXposition, startYposition);
				 goal->color = "blue";
	
				 Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
					 L"Assets/blue_box.png");
				 box->SetPosition(startXposition, startYposition);
				 box->StageInfo = &m_Stage2;
				 box->remainBoxCount = &remainBox;
				 box->boxIndex = i;
				 box->color = "blue";
				 box->indexRow = 10;
				 box->alreadyGoal = goal;
				 m_Stage2[i] = box;

			 }
		}
		else if (i / 10 == 3)
		{
			auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
				L"Assets/ground_06.png");
			pTile->SetPosition(startXposition, startYposition);

			if (i == 30 || i == 39)
			{
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			}
			if (i == 31)
			{
				Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
					L"Assets/dia_red.png");
				goal->SetPosition(startXposition, startYposition);
				goal->color = "red";
				m_Stage2[i] = goal;
			}
			if (i == 33)
			{
				Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
					L"Assets/red_snowbox.png");
				box->SetPosition(startXposition, startYposition);
				box->StageInfo = &m_Stage2;
				box->remainBoxCount = &remainBox;
				box->boxIndex = i;
				box->color = "red";
				box->indexRow = 10;
				m_Stage2[i] = box;
			}
			if (i == 34)
			{
				Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
					L"Assets/dia_brown.png");
				goal->SetPosition(startXposition, startYposition);
				goal->color = "brown";

				Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
					L"Assets/blue_box.png");
				box->SetPosition(startXposition, startYposition);
				box->StageInfo = &m_Stage2;
				box->remainBoxCount = &remainBox;
				box->boxIndex = i;
				box->color = "blue";
				box->indexRow = 10;
				box->alreadyGoal = goal;
				m_Stage2[i] = box;
			}
			if (i == 36)
			{
				Box* box = ActorManager::Instance().Create<Box>(static_cast<int>(Layer::Box),
					L"Assets/brown_box.png");
				box->SetPosition(startXposition, startYposition);
				box->StageInfo = &m_Stage2;
				box->remainBoxCount = &remainBox;
				box->boxIndex = i;
				box->color = "brown";
				box->indexRow = 10;
				m_Stage2[i] = box;
			}

		}
		else if (i / 10 == 4)
		{
			auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
				L"Assets/ground_06.png");
			pTile->SetPosition(startXposition, startYposition);
			
			if (i == 40 || i == 41 || i == 42 || i == 48 || i == 49)
			{
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			}
		}
		else if (i / 10 == 5)
		{
			if (i != 55)
			{
				auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
					L"Assets/ground_05.png");
				pTile->SetPosition(startXposition, startYposition);
			}
			else
			{
				auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
					L"Assets/ground_06.png");
				pTile->SetPosition(startXposition, startYposition);
			}
			if (i == 52 || i == 53 || i == 54 || i == 56 || i == 57 || i == 58 )
			{
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			}
			if (i == 55)
			{
				Goal* goal = ActorManager::Instance().Create<Goal>(static_cast<int>(Layer::Goal),
					L"Assets/dia_green.png");
				goal->SetPosition(startXposition, startYposition);
				goal->color = "green";
				m_Stage2[i] = goal;
			}
			
		}
		else if (i / 10 == 6)
		{
			auto pTile = ActorManager::Instance().Create<Actor>(static_cast<int>(Layer::Tile),
				L"Assets/ground_05.png");
			pTile->SetPosition(startXposition, startYposition);
			
			if (i == 64 || i == 65 || i == 66)
			{
				m_Stage2[i] = ActorManager::Instance().Create<Wall>(static_cast<int>(Layer::Wall),
					L"Assets/brown_block.png");
				m_Stage2[i]->SetPosition(startXposition, startYposition);
			}
		}

		startXposition += 64.0f;
		if (i % 10 == 9) { startYposition += 64.0f; startXposition = 120.0f; }

	}

}

void Game::Retry()
{
	m_Player->Reset();
	m_Player = nullptr;
	ActorManager::Instance().ReleaseAll();

	if (stageNum == 1)
	{
		m_Stage1.clear();
		Stage1Initialize();
	}
	else if (stageNum == 2)
	{
		m_Stage2.clear();
		Stage2Initialize();
	}
}

void Game::StageChange()
{
	Retry();
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
	std::wstring retrykey = L"Retry : R Key";
	m_fontPos2.x = 300.f;
	m_fontPos2.y = 540.f;

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());



	m_font->DrawString(m_spriteBatch.get(), output.c_str(),
		m_fontPos, Colors::White, 0.f);

	m_font->DrawString(m_spriteBatch.get(), retrykey.c_str(),
		m_fontPos2, Colors::White, 0.f);

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