#pragma once

#include "HGPI/Context.h"
#include "Backend/D3D11/D3D11Device.h"

namespace hgpi {

	struct D3D11PipelineState {
		// Input assembler
		size_t InputLayoutStride = 0;
		D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// Rasterizer
		D3D11_VIEWPORT Viewport = {};
		D3D11_RASTERIZER_DESC RasterizerDesc = {
			D3D11_FILL_SOLID, D3D11_CULL_NONE,
			true, false, 0, 0, true
		};
		ID3D11RasterizerState* RasterizerState = nullptr;

		// Output merger
		uint32_t RenderTargetCount = 0;

		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {
			true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
			false, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
			{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
			{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }
		};
		ID3D11DepthStencilState* DepthStencilState = nullptr;
	};

	class D3D11Context : public IContext, public DeviceChild<D3D11Device> {
	public:
		D3D11Context(D3D11Device* device);
		virtual ~D3D11Context();

		virtual void SetColorTargets(uint32_t targetCount, ColorTargetHandle* colorTargets) override;
		virtual void SetDepthTarget(const DepthTargetHandle& depthTarget) override;
		virtual void SetVertexBuffers(uint32_t vertexBufferCount, VertexBufferHandle* vertexBuffers, uint32_t* offsets) override;
		virtual void SetIndexBuffer(uint32_t indexOffset, IndexBufferHandle& indexBuffer) override;
		virtual void SetUniformBuffer(uint32_t binding, UniformBufferHandle& uniformBuffer, PipelineStage stageFlags) override;
		virtual void SetTexture(uint32_t binding, const TextureHandle& texture) override;
		virtual void SetBufferData(void* data, size_t size, size_t offset, const BufferHandle& buffer) override;
		virtual void SetShaderProgram(ShaderProgramHandle& shader) override;
		virtual void SetCulling(CullingMode mode) override;
		virtual void SetViewport(const Viewport& viewport) override;
		virtual void ClearColorTarget(ColorTargetHandle& colorTarget, float color[4]) override;
		virtual void ClearDepthTarget(DepthTargetHandle& depthTarget) override;
		virtual void Flush() override;
		virtual void Present(SwapchainHandle& swapchain, bool vsync) override;

		virtual void Draw(uint32_t vertexCount, uint32_t vertexOffset) override;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexOffset) override;

	protected:
		virtual void SubmitPipelineState() override;

	private:
		ID3D11DeviceContext* m_Context = nullptr,
			* m_ImmediateContext = nullptr;
		ID3D11CommandList* m_Commands = nullptr;

		D3D11PipelineState m_State;
	};
}