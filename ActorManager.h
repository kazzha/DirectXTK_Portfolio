#pragma once

namespace DX
{
	class ActorManager : public IDeviceNotify
	{
	public:
		static ActorManager& Instance()
		{
			static ActorManager instance;
			return instance;
		}

	public:
		ActorManager();
		~ActorManager();

		ActorManager(ActorManager&&) = delete;
		ActorManager& operator= (ActorManager&&) = delete;

		ActorManager(ActorManager const&) = delete;
		ActorManager& operator= (ActorManager const&) = delete;

	public:
		void OnDeviceLost() override;
		void OnDeviceRestored() override;

		void Initialize();
		void ReleaseAll();

		void Update(double dt);
		void Draw(DirectX::SpriteBatch* pSpriteBatch);

		template<class T>
		T* Create();

		template<class T>
		T* Create(
			const int layer,
			const LPCWSTR textureName,
			const LPCWSTR sheetName = nullptr,
			const LPCWSTR animName = nullptr
		);

		void Delete(int layer, Actor* pActor);

	private:
		std::map<int, std::list<std::unique_ptr<Actor>>> m_Actors;

	};

	template<class T>
	inline T* ActorManager::Create() // 빈 액터
	{
		m_Actors[0].push_back(std::make_unique<T>());
		return dynamic_cast<T*>(m_Actors[0].back().get());
	}

	template<class T>
	inline T* ActorManager::Create(
		const int layer,
		const LPCWSTR textureName,
		const LPCWSTR sheetName,
		const LPCWSTR animName
	)
	{
		m_Actors[layer].push_back(std::make_unique<T>());
		T* pActor = dynamic_cast<T*>(m_Actors[layer].back().get());

		pActor->LoadSpriteSheet(textureName, sheetName);
		if (animName != nullptr)
		{
			auto pAnim = dynamic_cast<AnimatedActor*>(pActor); // AnimatedActor라면 형변환
			if (pAnim)
			{
				pAnim->LoadAnimation(animName);
			}
		}

		return pActor;
	}
}


