#pragma once

#include "HGPI/Ref.h"

namespace hgpi {

	enum class BufferUsage {
		Static, Dynamic
	};

	// Provide (optional) initial data and usage hint to the device when creating a buffer
	struct BufferDesc {
		uint32_t Size;
		void* Data;
		BufferUsage Usage;
	};

	class IBuffer {
	public:
		virtual ~IBuffer() = default;
	};

	typedef Ref<IBuffer> BufferHandle;

	class IVertexBuffer : public IBuffer {
	public:
		virtual ~IVertexBuffer() = default;
	};

	typedef Ref<IVertexBuffer> VertexBufferHandle;

	class IIndexBuffer : public IBuffer {
	public:
		virtual ~IIndexBuffer() = default;
	};

	typedef Ref<IIndexBuffer> IndexBufferHandle;

	class IUniformBuffer : public IBuffer {
	public:
		virtual ~IUniformBuffer() = default;
	};

	typedef Ref<IUniformBuffer> UniformBufferHandle;
	typedef UniformBufferHandle ConstantBufferHandle;

	enum class FilterMode {
		Nearest = 1,
		Linear
	};

	enum class WrapMode {
		Repeat, Clamp
	};

	struct Texture2DDesc {
		uint32_t Width, Height, Channels;
		void* Data;
		BufferUsage Usage;
		FilterMode Filter;
		WrapMode Wrap;
	};

	class ITexture {
	public:
		virtual ~ITexture() = default;
	};

	typedef Ref<ITexture> TextureHandle;

	class ITexture2D : public ITexture {
	public:
		virtual ~ITexture2D() = default;
	};

	typedef Ref<ITexture2D> Texture2DHandle;

	class IColorTarget {
	public:
		virtual ~IColorTarget() = default;
	};

	typedef Ref<IColorTarget> ColorTargetHandle;

	class IDepthTarget {
	public:
		virtual ~IDepthTarget() = default;
	};

	typedef Ref<IDepthTarget> DepthTargetHandle;

}