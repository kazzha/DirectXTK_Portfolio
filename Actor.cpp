#include "pch.h"

using namespace DX;
using namespace DirectX;

#pragma warning(push)
#pragma warning(disable:26812)

Actor::Actor() :
	m_texture{},
	m_textureName{},
	m_jsonSheet{},
	m_sourceRect{0,0,1,1},
	m_pivot{0.f, 0.f},
	m_size{1.f, 1.f},
	m_position{0.f,0.f},
	m_rotation{0.0f},
	m_scale{1.0f},
	m_color{Colors::White},
	m_flip{SpriteEffects_None},
	m_layer{0.0f}
{
}

Actor::~Actor()
{
}

#pragma warning(pop)

void Actor::LoadSpriteSheet(const LPCWSTR textureFile, const LPCWSTR jsonFile)
{
	m_textureName = textureFile;
	m_texture = TextureManager::Instance().CreateShaderResourceView(m_textureName.c_str());

	if (jsonFile != nullptr)
	{
		m_jsonSheet = JsonManager::Instance().CreateDocument(jsonFile);
	}
	else
	{
		auto pTex = Utility::GetTextureFromSRV(m_texture);

		D3D11_TEXTURE2D_DESC desc;
		pTex->GetDesc(&desc);

		m_size.x = static_cast<float>(desc.Width);
		m_size.y = static_cast<float>(desc.Height);
		m_pivot.x = m_size.x / 2;
		m_pivot.y = m_size.y / 2;
		m_sourceRect.left = 0;
		m_sourceRect.top = 0;
		m_sourceRect.right = static_cast<LONG>(m_size.x);
		m_sourceRect.bottom = static_cast<LONG>(m_size.y);
	}
}

void Actor::SetFrame(const LPCWSTR frameName)
{
	if (m_jsonSheet != nullptr)
	{
		const auto& frames = (*m_jsonSheet)[L"frames"].GetArray();
		for (auto& elem : frames)
		{
			if (std::wcscmp(elem[L"filename"].GetString(), frameName) == 0)
			{
				const auto& obj = elem[L"frame"].GetObject();
				m_size.x = static_cast<float>(obj[L"w"].GetInt());
				m_size.y = static_cast<float>(obj[L"h"].GetInt());
				m_pivot.x = m_size.x / 2;
				m_pivot.y = m_size.y / 2;
				m_sourceRect.left = static_cast<LONG>(obj[L"x"].GetInt());
				m_sourceRect.top = static_cast<LONG>(obj[L"y"].GetInt());
				m_sourceRect.right = m_sourceRect.left + static_cast<LONG>(m_size.x);
				m_sourceRect.bottom = m_sourceRect.top + static_cast<LONG>(m_size.y);

				return;
			}
		}

		Utility::DebugString(L"[ACTOR] : Can't find frame %s", frameName);

	}
}

void Actor::OnDeviceLost()
{
	m_texture = nullptr;
}

void Actor::OnDeviceRestored()
{
	m_texture = TextureManager::Instance().CreateShaderResourceView(m_textureName.c_str());
}

void Actor::Draw(DirectX::SpriteBatch* batch)
{
	if (batch == nullptr || m_texture == nullptr)
		return;

	XMFLOAT2 pivot = m_pivot;
	if (m_flip & SpriteEffects_FlipHorizontally)
	{
		pivot.x = m_size.x - pivot.x;
	}
	if (m_flip & SpriteEffects_FlipVertically)
	{
		pivot.y = m_size.y - pivot.y;
	}

	batch->Draw(
		m_texture,
		m_position,
		&m_sourceRect,
		m_color,
		m_rotation,
		pivot,
		m_scale,
		m_flip,
		m_layer
	);
}
