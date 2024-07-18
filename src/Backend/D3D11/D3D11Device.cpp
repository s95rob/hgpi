#include "Backend/D3D11/D3D11Device.h"
#include "Backend/D3D11/D3D11Context.h"
#include "Backend/D3D11/D3D11Swapchain.h"
#include "Backend/D3D11/D3D11Buffer.h"
#include "Backend/D3D11/D3D11Shader.h"

namespace hgpi {

	D3D11Device::D3D11Device(const DeviceDesc& desc) {
		int flags = 0;
		if (desc.DebugEnabled)
			flags |= D3D11_CREATE_DEVICE_DEBUG;

		DX_THROW(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			flags, nullptr, 0, D3D11_SDK_VERSION, &m_Device, nullptr, nullptr));

		DX_THROW(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_Factory));
	}

	D3D11Device::~D3D11Device() {
		if (m_Device)
			m_Device->Release();
		if (m_Factory)
			m_Factory->Release();
	}

	ContextHandle D3D11Device::CreateContext() {
		return MakeRef<D3D11Context>(this);
	}

	SwapchainHandle D3D11Device::CreateSwapchain(const SwapchainDesc& desc) {
		return MakeRef<D3D11Swapchain>(this, desc);
	}

	DepthTargetHandle D3D11Device::CreateDepthTarget(const Texture2DDesc& desc) {
		return MakeRef<D3D11DepthTarget>(this, desc);
	}

	VertexBufferHandle D3D11Device::CreateVertexBuffer(const BufferDesc& desc) {
		return MakeRef<D3D11VertexBuffer>(this, desc);
	}

	IndexBufferHandle  D3D11Device::CreateIndexBuffer(const BufferDesc& desc) {
		return MakeRef<D3D11IndexBuffer>(this, desc);
	}

	UniformBufferHandle D3D11Device::CreateUniformBuffer(const BufferDesc& desc) {
		return MakeRef<D3D11ConstantBuffer>(this, desc);
	}

	Texture2DHandle D3D11Device::CreateTexture2D(const Texture2DDesc& desc) {
		return MakeRef<D3D11Texture2D>(this, desc);
	}

	ShaderProgramHandle D3D11Device::CreateShaderProgram(const ShaderProgramDesc& desc) {
		return MakeRef<D3D11ShaderProgram>(this, desc);
	}

}