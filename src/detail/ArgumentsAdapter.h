#pragma once

#include "always_false.h"
#include "charconv_hack.h"
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace detail
{
class ArgumentsAdapter
{
public:
	ArgumentsAdapter(const std::vector<std::string>& arguments)
		: m_arguments(arguments)
	{
	}

	void CheckArgumentsCount(size_t expectedCount) const
	{
		if (expectedCount != m_arguments.size())
		{
			const std::string expectedStr = std::to_string(expectedCount);
			const std::string gotStr = std::to_string(m_arguments.size());
			throw std::invalid_argument("Argument count mismatch: "
				+ expectedStr + " expected, got " + gotStr);
		}
	}

	template<class T>
	void operator()(T& destination, size_t index) const
	{
		using namespace std::literals;
		if (!Convert(destination, m_arguments.at(index)))
		{
			throw std::runtime_error(
				"expected type "s + typeid(T).name()
				+ " for argument #"s + std::to_string(index));
		}
	}

private:
	template<class T>
	bool Convert(T&& destination, const std::string& arg) const
	{
		using DT = std::decay_t<T>;
		// TODO: add `bool` support
		if constexpr (std::is_integral_v<DT>)
		{
			[[maybe_unused]] auto[ptr, err]
				= std::from_chars(arg.data(), arg.data() + arg.size(), destination);
			return !err;
		}
		else if constexpr (std::is_same_v<DT, std::string>)
		{
			destination = arg;
			return true;
		}
		else
		{
			static_assert(detail::always_false<T>, "argument conversion is not implemented for type T");
		}
	}

	std::vector<std::string> m_arguments;
};
} // namespace detail
