#pragma once

#include "HGPI/Ref.h"
#include "HGPI/Swapchain.h"
#include "HGPI/Shader.h"

namespace hgpi {

	enum PipelineStage {
		HGPI_STAGE_VERTEX_SHADER_BIT = 1,
		HGPI_STAGE_FRAGMENT_SHADER_BIT = 2,
		HGPI_STAGE_PIXEL_SHADER_BIT = HGPI_STAGE_FRAGMENT_SHADER_BIT
	};

	struct Viewport {
		float X, Y, Width, Height, MinDepth, MaxDepth;
	};

	enum class CullingMode {
		None = 0,
		Back,
		Front
	};

	class IContext {
	public:
		virtual ~IContext() = default;

		// Set the pipeline's ColorTarget(s)
		virtual void SetColorTargets(uint32_t targetCount, ColorTargetHandle* colorTargets) = 0;

		// Set the pipeline's DepthTarget (pass nullptr to disable depth testing)
		virtual void SetDepthTarget(const DepthTargetHandle& depthTarget) = 0;
		virtual void SetVertexBuffers(uint32_t vertexBufferCount, VertexBufferHandle* vertexBuffers, uint32_t* offsets) = 0;
		virtual void SetIndexBuffer(uint32_t indexOffset, IndexBufferHandle& indexBuffer) = 0;
		virtual void SetUniformBuffer(uint32_t binding, UniformBufferHandle& uniformBuffer, PipelineStage stageFlags) = 0;
		virtual void SetTexture(uint32_t binding, const TextureHandle& texture) = 0;
		virtual void SetBufferData(void* data, size_t size, size_t offset, const BufferHandle& buffer) = 0;
		virtual void SetShaderProgram(ShaderProgramHandle& shader) = 0;

		virtual void SetViewport(const Viewport& viewport) = 0;
		virtual void SetCulling(CullingMode mode) = 0;
		virtual void ClearColorTarget(ColorTargetHandle& colorTarget, float color[4]) = 0;
		virtual void ClearDepthTarget(DepthTargetHandle& depthTarget) = 0;

		virtual void Draw(uint32_t vertexCount, uint32_t vertexOffset) = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexOffset) = 0;

		// Flushes commands to the GPU and submits them for execution
		virtual void Flush() = 0;

		// Advances the swapchain image and presents it to the window
		virtual void Present(SwapchainHandle& swapchain, bool vsync = false) = 0;

	protected:
		// Writes the persistent pipeline state to the command buffer
		virtual void SubmitPipelineState() = 0;
	};

	typedef Ref<IContext> ContextHandle;

}