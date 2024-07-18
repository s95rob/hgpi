#pragma once

#include "HGPI/Ref.h"
#include "HGPI/Context.h"
#include "HGPI/Swapchain.h"
#include "HGPI/Shader.h"

namespace hgpi {

	enum class API {
		None = 0,
		D3D11
	};

	struct DeviceDesc {
		API Backend;
		bool DebugEnabled;
	};

	class IDevice {
	public:
		virtual ~IDevice() = default;

		virtual ContextHandle CreateContext() = 0;
		virtual SwapchainHandle CreateSwapchain(const SwapchainDesc& desc) = 0;
		virtual DepthTargetHandle CreateDepthTarget(const Texture2DDesc& desc) = 0;
		virtual VertexBufferHandle CreateVertexBuffer(const BufferDesc& desc) = 0;
		virtual IndexBufferHandle CreateIndexBuffer(const BufferDesc& desc) = 0;
		virtual UniformBufferHandle CreateUniformBuffer(const BufferDesc& desc) = 0;
		ConstantBufferHandle CreateConstantBuffer(const BufferDesc& desc) { this->CreateUniformBuffer(desc); }
		virtual Texture2DHandle CreateTexture2D(const Texture2DDesc& desc) = 0;
		virtual ShaderProgramHandle CreateShaderProgram(const ShaderProgramDesc& desc) = 0;
	};

	typedef Ref<IDevice> DeviceHandle;

	template <typename ParentType>
	class DeviceChild {
	public:
		DeviceChild(ParentType* device)
			: m_Device(device) {}

		virtual ParentType* GetDevice() { return m_Device; }

	private:
		ParentType* m_Device = nullptr;
	};

	DeviceHandle CreateDevice(const DeviceDesc& desc);
}