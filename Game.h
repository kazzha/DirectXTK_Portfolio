#pragma once

using namespace DX;

class Game : public DX::IDeviceNotify
{
public:
	Game() noexcept(false);
	virtual ~Game();

	Game(Game&&) = default;
	Game& operator= (Game&&) = default;

	Game(Game const&) = delete;
	Game& operator= (Game const&) = delete;

	virtual void Initialize(HWND window, int width, int height);

	virtual void Tick();

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();
	virtual void OnWindowMoved();
	virtual void OnWindowSizeChanged(int width, int height);
	void processInput(WPARAM wParam);
	void Stage1Initialize();
	void Stage2Initialize();
	void Retry();
	void StageChange();
	void StageChangeFont();


	virtual void GetDefaultSize(int& width, int& height) const noexcept;

protected:
	virtual void Update(DX::StepTimer const& timer);
	virtual void Render();

	virtual void Clear();

	virtual void CreateDeviceDependentResources();
	virtual void CreateWindowSizeDependentResources();

	std::unique_ptr<DX::DeviceResources> m_deviceResources;

	DX::StepTimer m_timer;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	std::unique_ptr<DirectX::SpriteFont> m_font;
	DirectX::SimpleMath::Vector2 m_fontPos;
	DirectX::SimpleMath::Vector2 m_fontPos2;

	DX::Player* m_Player;
	std::vector<Actor*> m_Stage1;
	std::vector<Actor*> m_Stage2;
	int walkCount;
	int remainBox;
	int stageNum;
	double AccumulateTime;
};

