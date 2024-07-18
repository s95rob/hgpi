#pragma once

#include "HGPI/Buffer.h"
#include "Backend/D3D11/D3D11Device.h"

namespace hgpi {

	class D3D11Buffer : public DeviceChild<D3D11Device> {
	public:
		D3D11Buffer(D3D11Device* device, const BufferDesc& desc, D3D11_BIND_FLAG bind);
		virtual ~D3D11Buffer();

		ID3D11Buffer* GetBuffer() { return m_Buffer; }

		operator ID3D11Buffer*() { return m_Buffer; }

	protected:
		ID3D11Buffer* m_Buffer = nullptr;
	};

	class D3D11VertexBuffer : public IVertexBuffer, public D3D11Buffer {
	public:
		D3D11VertexBuffer(D3D11Device* device, const BufferDesc& desc)
			: D3D11Buffer(device, desc, D3D11_BIND_VERTEX_BUFFER) {}
		virtual ~D3D11VertexBuffer() = default;

	};

	class D3D11IndexBuffer : public IIndexBuffer, public D3D11Buffer {
	public:
		D3D11IndexBuffer(D3D11Device* device, const BufferDesc& desc)
			: D3D11Buffer(device, desc, D3D11_BIND_INDEX_BUFFER) {}
		virtual ~D3D11IndexBuffer() = default;
	};

	class D3D11ConstantBuffer : public IUniformBuffer, public D3D11Buffer {
	public:
		D3D11ConstantBuffer(D3D11Device* device, const BufferDesc& desc)
			: D3D11Buffer(device, desc, D3D11_BIND_CONSTANT_BUFFER) {}
		virtual ~D3D11ConstantBuffer() = default;
	};

	class D3D11Texture : public ITexture, public DeviceChild<D3D11Device> {
	public:
		D3D11Texture(D3D11Device* device)
			: DeviceChild(device) {}
		virtual ~D3D11Texture() = default;

		ID3D11SamplerState* GetSampler() { return m_Sampler; }
		ID3D11ShaderResourceView* GetTextureView() { return m_TextureView; }

		virtual operator ID3D11Resource*() = 0;

	protected:
		ID3D11SamplerState* m_Sampler = nullptr;
		ID3D11ShaderResourceView* m_TextureView = nullptr;
		D3D11_SAMPLER_DESC m_SamplerDesc;
	};

	class D3D11Texture2D : public ITexture2D, public D3D11Texture {
	public:
		D3D11Texture2D(D3D11Device* device, const Texture2DDesc& desc, D3D11_BIND_FLAG bindFlags);
		D3D11Texture2D(D3D11Device* device, const Texture2DDesc& desc)
			: D3D11Texture2D(device, desc, D3D11_BIND_SHADER_RESOURCE) {}
		virtual ~D3D11Texture2D();

		virtual operator ID3D11Resource*() override { return m_Texture; }

	protected:
		ID3D11Texture2D* m_Texture = nullptr;
	};

	class D3D11ColorTarget : public IColorTarget {
	public:
		D3D11ColorTarget() = default;
		D3D11ColorTarget(ID3D11RenderTargetView* renderTarget)
			: m_RenderTarget(renderTarget) {}
		virtual ~D3D11ColorTarget() { m_RenderTarget->Release(); }

		operator ID3D11RenderTargetView*() { return m_RenderTarget; }

	private:
		ID3D11RenderTargetView* m_RenderTarget = nullptr;
	};

	class D3D11DepthTarget : public IDepthTarget, public D3D11Texture2D {
	public:
		D3D11DepthTarget(D3D11Device* device, const Texture2DDesc& desc);
		virtual ~D3D11DepthTarget();

		operator ID3D11DepthStencilView*() { return m_DepthView; }

	protected:
		ID3D11DepthStencilView* m_DepthView = nullptr;
	};

}