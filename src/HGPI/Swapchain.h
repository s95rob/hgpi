#pragma once

#include "HGPI/Buffer.h"

namespace hgpi {

	struct SwapchainDesc {
		uint32_t BufferCount,
			Width, Height;
		void* Window = nullptr;
	};

	class ISwapchain {
	public:
		virtual ~ISwapchain() = default;

		virtual ColorTargetHandle GetBackbuffer() = 0;
	};

	typedef Ref<ISwapchain> SwapchainHandle;
}