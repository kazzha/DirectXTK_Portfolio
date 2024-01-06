#pragma once

namespace DX
{
	class Utility
	{
	public:

		static ID3D11Texture2D* GetTextureFromSRV(ID3D11ShaderResourceView* pSRV);

		static void DebugString(LPCWSTR fmt, ...);
	};
}


