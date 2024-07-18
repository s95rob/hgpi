#pragma once

#include <memory>

namespace hgpi {

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename D, typename T>
	constexpr Ref<D> ResolveRef(const Ref<T>& ref) {
		return std::dynamic_pointer_cast<D>(ref);
	}
}