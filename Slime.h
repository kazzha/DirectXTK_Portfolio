#pragma once
#include "AnimatedActor.h"

namespace DX
{
	class Slime : public AnimatedActor
	{
		const DirectX::XMFLOAT2 UPVECTOR{ 0.0f, -1.0f };
		float mStep{};

	public:
		virtual void Update(double delta) override;
		void Move();
	};
}


