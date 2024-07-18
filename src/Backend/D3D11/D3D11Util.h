#pragma once

#include "D3D11Include.h"

#include "HGPI/Shader.h"

namespace hgpi {

	inline constexpr DXGI_FORMAT DXGIFormatFromAttrib(const VertexAttrib& attrib) {
		switch (attrib.Type) {
		case DataType::Float: {
			switch (attrib.ComponentCount) {
			case 1: return DXGI_FORMAT_R32_FLOAT;
			case 2: return DXGI_FORMAT_R32G32_FLOAT;
			case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
			case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		} 
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	inline constexpr DXGI_FORMAT DXGIFormatFromComponentCount(uint32_t count) {
		switch (count) {
		case 1: return DXGI_FORMAT_R8_UNORM;
		case 2: return DXGI_FORMAT_R8G8_UNORM;
		case 3: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case 4: return DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	inline constexpr D3D11_FILTER D3D11FilterFromFilterMode(FilterMode filter) {
		switch (filter) {
		case FilterMode::Nearest: return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case FilterMode::Linear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}

		return (D3D11_FILTER)0;
	}

	inline constexpr D3D11_TEXTURE_ADDRESS_MODE D3D11AddressModeFromWrapMode(WrapMode wrap) {
		switch (wrap) {
		case WrapMode::Repeat: return D3D11_TEXTURE_ADDRESS_WRAP;
		case WrapMode::Clamp: return D3D11_TEXTURE_ADDRESS_CLAMP;
		}

		return (D3D11_TEXTURE_ADDRESS_MODE)0;
	}

	inline constexpr D3D11_CULL_MODE D3D11CullModeFromCullingMode(CullingMode mode) {
		switch (mode) {
		case CullingMode::None: return D3D11_CULL_NONE;
		case CullingMode::Back: return D3D11_CULL_BACK;
		case CullingMode::Front: return D3D11_CULL_FRONT;
		}

		return (D3D11_CULL_MODE)0;
	}

	inline constexpr D3D11_USAGE D3D11UsageFromBufferUsage(BufferUsage usage) {
		switch (usage) {
		case BufferUsage::Static: return D3D11_USAGE_DEFAULT;
		case BufferUsage::Dynamic: return D3D11_USAGE_DYNAMIC;
		}

		return (D3D11_USAGE)0;
	}

	inline constexpr UINT D3D11CPUAccessFromBufferUsage(BufferUsage usage) {
		switch (usage) {
		case BufferUsage::Static: return 0;
		case BufferUsage::Dynamic: return D3D11_CPU_ACCESS_WRITE;
		}

		return 0;
	}

}