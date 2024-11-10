#pragma once

#include <memory>
#include "Denix/Core/Logger.h"

namespace Denix
{
	// Smart Pointers
	template <typename T>
	using Ref = std::shared_ptr<T>;

	
	/**
	 * 
	 * @tparam T 
	 * @tparam Args 
	 * @param args 
	 * @return 
	 */
	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	

	template <typename T>
	using URef = std::unique_ptr<T>;

	/**
	 * 
	 * @tparam T 
	 * @tparam Args 
	 * @param args 
	 * @return 
	 */
	template<typename T, typename ... Args>
	constexpr URef<T> MakeURef(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using WRef = std::weak_ptr<T>;
	
	/**
	 * 
	 * @tparam T 
	 * @tparam Arg 
	 * @param arg 
	 * @return 
	 */
	template<typename T, typename Arg>
	constexpr Ref<T> CastRef(Arg&& arg)
	{
		return std::dynamic_pointer_cast<T>(std::forward<Arg>(arg));
	}
	
	// Pointer validation
	/**
	 * 
	 * @tparam T 
	 * @param _ref 
	 * @return 
	 */
	template<typename T>
	constexpr bool IsValid(const Ref<T>& _ref)
	{
		return _ref.get() != nullptr;
	}

	
}
