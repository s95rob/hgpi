#pragma once

#include "HGPI/Device.h"

#include "D3D11Include.h"

namespace hgpi {

	class D3D11Device : public IDevice {
	public:
		D3D11Device(const DeviceDesc& desc);
		virtual ~D3D11Device();

		virtual ContextHandle CreateContext() override;
		virtual SwapchainHandle CreateSwapchain(const SwapchainDesc& desc) override;
		virtual DepthTargetHandle CreateDepthTarget(const Texture2DDesc& desc) override;
		virtual VertexBufferHandle CreateVertexBuffer(const BufferDesc& desc) override;
		virtual IndexBufferHandle CreateIndexBuffer(const BufferDesc& desc) override;
		virtual UniformBufferHandle CreateUniformBuffer(const BufferDesc& desc) override;
		virtual Texture2DHandle CreateTexture2D(const Texture2DDesc& desc) override;
		virtual ShaderProgramHandle CreateShaderProgram(const ShaderProgramDesc& desc) override;

		IDXGIFactory* GetDXGIFactory() { return m_Factory; }

		operator ID3D11Device*() { return m_Device; }

	private:
		ID3D11Device* m_Device = nullptr;
		IDXGIFactory* m_Factory = nullptr;
	};

}