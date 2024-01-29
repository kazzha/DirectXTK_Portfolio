#pragma once

namespace DX 
{
	class Actor : public IDeviceNotify
	{
	public:
		Actor();
		virtual ~Actor();

		void LoadSpriteSheet(const LPCWSTR textureFile, const LPCWSTR jsonFile = nullptr);
		void SetFrame(const LPCWSTR frameName);
		void SetPosition(float x, float y) { m_position.x = x; m_position.y = y; }
		DirectX::XMFLOAT2 XM_CALLCONV GetPosition() { return m_position; }
		void SetPivot(float x, float y) { m_pivot.x = x; m_pivot.y = y; }
		void SetRotation(float r) { m_rotation = r; }

		virtual void OnDeviceLost() override;
		virtual void OnDeviceRestored() override;
		virtual void Update(double delta) {}
		virtual void Draw(DirectX::SpriteBatch* batch);
		
	protected:
		DirectX::XMFLOAT2 m_position;
		DirectX::XMVECTORF32 m_color;
		float m_rotation;
		float m_scale;
		DirectX::SpriteEffects m_flip;
		float m_layer;

	private:
		ID3D11ShaderResourceView* m_texture;
		std::wstring m_textureName;
		rapidjson::WDocument* m_jsonSheet;
		DirectX::XMFLOAT2 m_pivot;
		DirectX::XMFLOAT2 m_size;
		RECT m_sourceRect;
	};
}

