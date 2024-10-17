#pragma once

#include <memory>
#include "Denix/Core/Logger.h"

namespace Denix
{
	// Smart Pointers
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename Arg>
	constexpr Ref<T> CastRef(Arg&& arg)
	{
		return std::dynamic_pointer_cast<T>(std::forward<Arg>(arg));
	}

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using WeakRef = std::weak_ptr<T>;
	
	template<typename T, typename ... Args>
	constexpr WeakRef<T> MakeWeakRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	
	// Pointer validation
	template<typename T>
	constexpr bool IsValid(const Ref<T>& _ref)
	{
		return _ref.get() != nullptr;
	}

	
}
