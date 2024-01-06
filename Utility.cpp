#include "pch.h"
#include <strsafe.h>
#include <array>

namespace DX
{ 
    ID3D11Texture2D* Utility::GetTextureFromSRV(ID3D11ShaderResourceView* pSRV)
    {
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> textureInterface;

        assert(pSRV != nullptr);
        pSRV->GetResource(resource.GetAddressOf());
        assert(resource != nullptr);
        resource->QueryInterface<ID3D11Texture2D>(
            textureInterface.GetAddressOf()
            );
        assert(textureInterface != nullptr);
        return textureInterface.Get();
    }

    void Utility::DebugString(LPCWSTR fmt, ...)
    {
#ifdef  _DEBUG
        //static wchar_t error[1024];
        static std::array<wchar_t, 1024> error;

        error.fill('\0');

        va_list va;
        va_start(va, fmt);
        int size = std::vswprintf(&error[0], 1024, fmt, va);
        if (size < 1024)
        {
            error.at(size) = L'\n';
        }
        OutputDebugString(&error[0]);

        va_end(va);
#endif //  _DEBUG

    }

}
