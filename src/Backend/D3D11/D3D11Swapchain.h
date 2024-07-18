#pragma once

#include "HGPI/Swapchain.h"

#include "Backend/D3D11/D3D11Device.h"

namespace hgpi {

	class D3D11Swapchain : public ISwapchain, public DeviceChild<D3D11Device> {
	public:
		D3D11Swapchain(D3D11Device* device, const SwapchainDesc& desc);
		virtual ~D3D11Swapchain();

		virtual ColorTargetHandle GetBackbuffer();

		operator IDXGISwapChain*() { return m_Swapchain; }

	private:
		IDXGISwapChain* m_Swapchain = nullptr;
	};

}