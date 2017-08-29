#pragma once
#include <functional>

namespace detail
{
/////////////////////////////////////////////////////////////////////////////////////////////////
// Function traits keeps `std::function<>` typedef for any callable.
// Supported callables:
//  - lambdas
//  - functions
//  - methods (const, non-const)
//  - std::function
// Unsupported:
//  - class returned by std::bind
//  - generic lambdas

// For generic types that are functors, delegate to its 'operator()'
// Does not work for result of std::bind.
template<typename T>
struct function_traits : public function_traits<decltype(&T::operator())>
{
};

// For pointers to const member function.
template<typename ClassType, typename Result, typename... Args>
struct function_traits<Result (ClassType::*)(Args...) const>
{
	using function = std::function<Result(Args...)>;
};

// For pointers to member function.
template<typename ClassType, typename Result, typename... Args>
struct function_traits<Result (ClassType::*)(Args...)>
{
	using function = std::function<Result(Args...)>;
};

// For function pointers.
template<typename Result, typename... Args>
struct function_traits<Result (*)(Args...)>
{
	using function = std::function<Result(Args...)>;
};

// Returns std::function for any callable.
template<typename Callable>
auto make_function(Callable&& callable)
{
	using function = typename function_traits<std::decay_t<Callable>>::function;
	return function(callable);
}

} // namespace cpsutils
