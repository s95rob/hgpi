#include "Backend/D3D11/D3D11Context.h"
#include "Backend/D3D11/D3D11Buffer.h"
#include "Backend/D3D11/D3D11Swapchain.h"
#include "Backend/D3D11/D3D11Shader.h"
#include "Backend/D3D11/D3D11Util.h"

#include <vector>
#include <iostream>

namespace hgpi {

	D3D11Context::D3D11Context(D3D11Device* device)
		: DeviceChild(device) {
		ID3D11Device* dev = *device;
		DX_THROW(dev->CreateDeferredContext(0, &m_Context));
		dev->GetImmediateContext(&m_ImmediateContext);

		this->SubmitPipelineState();
	}

	D3D11Context::~D3D11Context() {
		m_Context->Release();
	}

	void D3D11Context::SetColorTargets(uint32_t targetCount, ColorTargetHandle* colorTargets) {
		std::vector<ID3D11RenderTargetView*> rtvs(targetCount);
		for (int i = 0; i < targetCount; i++) {
			rtvs[i] = *ResolveRef<D3D11ColorTarget>(colorTargets[i]);
		}
		ID3D11DepthStencilView* dsv = nullptr;
		m_Context->OMGetRenderTargets(0, nullptr, &dsv);
		m_Context->OMSetRenderTargets(targetCount, rtvs.data(), dsv);
		m_State.RenderTargetCount = targetCount;
	}

	void D3D11Context::SetDepthTarget(const DepthTargetHandle& depthTarget) {
		ID3D11RenderTargetView* rtvs = nullptr;
		m_Context->OMGetRenderTargets(m_State.RenderTargetCount, &rtvs, nullptr);
		if (depthTarget != nullptr) {
			ID3D11DepthStencilView* dt = *ResolveRef<D3D11DepthTarget>(depthTarget);
			m_Context->OMSetRenderTargets(m_State.RenderTargetCount, &rtvs, dt);
			m_State.DepthStencilDesc.DepthEnable = true;
		}
		else {
			m_Context->OMSetRenderTargets(m_State.RenderTargetCount, &rtvs, nullptr);
			m_State.DepthStencilDesc.DepthEnable = false;
		}
		ID3D11Device* device = *this->GetDevice();
		device->CreateDepthStencilState(&m_State.DepthStencilDesc, &m_State.DepthStencilState);
		m_Context->OMSetDepthStencilState(m_State.DepthStencilState, 0);
	}

	void D3D11Context::SetVertexBuffers(uint32_t vertexBufferCount, VertexBufferHandle* vertexBuffers, uint32_t* offsets) {
		std::vector<ID3D11Buffer*> vbs(vertexBufferCount);
		for (int i = 0; i < vertexBufferCount; i++) {
			vbs[i] = *ResolveRef<D3D11VertexBuffer>(vertexBuffers[i]);
		}
		std::vector<uint32_t> defaultOffsets(vertexBufferCount);
		defaultOffsets.assign(vertexBufferCount, 0);
		m_Context->IASetVertexBuffers(0, vbs.size(), vbs.data(), (UINT*)&m_State.InputLayoutStride, !offsets ? defaultOffsets.data() : offsets);
	}

	void D3D11Context::SetIndexBuffer(uint32_t indexOffset, IndexBufferHandle& indexBuffer) {
		auto& ib = ResolveRef<D3D11IndexBuffer>(indexBuffer);
		m_Context->IASetIndexBuffer(*ib, DXGI_FORMAT_R32_UINT, indexOffset);
	}

	void D3D11Context::SetUniformBuffer(uint32_t binding, UniformBufferHandle& uniformBuffer, PipelineStage stageFlags) {
		ID3D11Buffer* ub = *ResolveRef<D3D11ConstantBuffer>(uniformBuffer);
		if (stageFlags & HGPI_STAGE_VERTEX_SHADER_BIT)
			m_Context->VSSetConstantBuffers(binding, 1, &ub);
		if (stageFlags & HGPI_STAGE_FRAGMENT_SHADER_BIT)
			m_Context->PSSetConstantBuffers(binding, 1, &ub);
	}

	void D3D11Context::SetTexture(uint32_t binding, const TextureHandle& texture) {
		auto& t = ResolveRef<D3D11Texture>(texture);
		auto* view = t->GetTextureView();
		auto* sampler = t->GetSampler();
		m_Context->PSSetShaderResources(binding, 1, &view);
		m_Context->PSSetSamplers(binding, 1, &sampler);
	}

	void D3D11Context::SetBufferData(void* data, size_t size, size_t offset, const BufferHandle& buffer) {
		auto& b = std::dynamic_pointer_cast<D3D11Buffer>(buffer);
		D3D11_MAPPED_SUBRESOURCE msr;
		m_Context->Map(*b, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		std::memcpy((void*)((size_t)msr.pData + offset), data, size);
		m_Context->Unmap(*b, 0);
	}

	void D3D11Context::SetShaderProgram(ShaderProgramHandle& shader) {
		auto& sp = ResolveRef<D3D11ShaderProgram>(shader);
		m_Context->VSSetShader(sp->GetVertexShader(), nullptr, 0);
		m_Context->PSSetShader(sp->GetPixelShader(), nullptr, 0);
		m_Context->IASetInputLayout(sp->GetInputLayout());
		m_State.InputLayoutStride = sp->GetInputLayoutStride();
	}

	void D3D11Context::SetViewport(const Viewport& viewport) {
		m_State.Viewport = { viewport.X, viewport.Y, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth };
		m_Context->RSSetViewports(1, &m_State.Viewport);
	}

	void D3D11Context::SetCulling(CullingMode mode) {
		m_State.RasterizerDesc.CullMode = D3D11CullModeFromCullingMode(mode);
		ID3D11Device* device = *this->GetDevice();
		device->CreateRasterizerState(&m_State.RasterizerDesc, &m_State.RasterizerState);
		m_Context->RSSetState(m_State.RasterizerState);
	}

	void D3D11Context::ClearColorTarget(ColorTargetHandle& colorTarget, float color[4]) { 
		ID3D11RenderTargetView* rtv = *ResolveRef<D3D11ColorTarget>(colorTarget);
		m_Context->ClearRenderTargetView(rtv, color);
	}

	void D3D11Context::ClearDepthTarget(DepthTargetHandle& depthTarget) {
		ID3D11DepthStencilView* dv = *ResolveRef<D3D11DepthTarget>(depthTarget);
		m_Context->ClearDepthStencilView(dv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void D3D11Context::Flush() { 
		DX_THROW(m_Context->FinishCommandList(false, &m_Commands));
		m_ImmediateContext->ExecuteCommandList(m_Commands, false);
		m_Commands->Release();
		this->SubmitPipelineState();
		m_State.InputLayoutStride = 0;
	}

	void D3D11Context::Present(SwapchainHandle& swapchain, bool vsync) {
		IDXGISwapChain* sc = *ResolveRef<D3D11Swapchain>(swapchain);
		this->Flush();
		DX_THROW(sc->Present(vsync ? 1 : 0, 0));
	}

	void D3D11Context::Draw(uint32_t vertexCount, uint32_t vertexOffset) {
		m_Context->Draw(vertexCount, vertexOffset);
	}

	void D3D11Context::DrawIndexed(uint32_t indexCount, uint32_t indexOffset) {
		m_Context->DrawIndexed(indexCount, indexOffset, 0);
	}

	void D3D11Context::SubmitPipelineState() {
		m_Context->IASetPrimitiveTopology(m_State.Topology);
		m_Context->RSSetViewports(1, &m_State.Viewport);
		
		ID3D11Device* device = *this->GetDevice();
		// If the driver finds an existing state object matching the descriptor, it will
		// return that state object instead of creating a new one
		device->CreateRasterizerState(&m_State.RasterizerDesc, &m_State.RasterizerState);
		m_Context->RSSetState(m_State.RasterizerState);

	}
}