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
	DX::Player* m_Player;
	std::vector<Actor*> m_Object;
};

