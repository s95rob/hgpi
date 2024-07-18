#include "Backend/D3D11/D3D11Swapchain.h"
#include "Backend/D3D11/D3D11Buffer.h"

namespace hgpi {

	D3D11Swapchain::D3D11Swapchain(D3D11Device* device, const SwapchainDesc& desc)
		: DeviceChild<D3D11Device>(device) {
		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferCount = desc.BufferCount;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Width = desc.Width;
		scd.BufferDesc.Height = desc.Height;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = static_cast<HWND>(desc.Window);
		scd.SampleDesc.Count = 1;
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		DX_THROW(device->GetDXGIFactory()->CreateSwapChain(*device, &scd, &m_Swapchain));
	}

	D3D11Swapchain::~D3D11Swapchain() {
		m_Swapchain->Release();
	}

	ColorTargetHandle D3D11Swapchain::GetBackbuffer() {
		ID3D11Texture2D* backbufferImage = nullptr;
		ID3D11RenderTargetView* rtv = nullptr;
		m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferImage);
		ID3D11Device* device = *this->GetDevice();
		DX_THROW(device->CreateRenderTargetView(backbufferImage, nullptr, &rtv));
		backbufferImage->Release();
		
		return MakeRef<D3D11ColorTarget>(rtv);
	}
}