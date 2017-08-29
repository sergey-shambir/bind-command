#pragma once
#include <string>
#include <type_traits>

namespace detail
{
struct ResultConverter
{
	template<class T>
	static std::string Convert(T&& value, std::enable_if_t<std::is_integral_v<T>>* = nullptr)
	{
		return std::to_string(value);
	}

	template<class T>
	static std::string Convert(T&& value, std::enable_if_t<std::is_same_v<T, std::string>>* = nullptr)
	{
		return std::to_string(std::move(value));
	}
};
} // namespace detail
