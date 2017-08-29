#pragma once

#include "detail/ArgumentsAdapter.h"
#include "detail/BindCommand_detail.h"
#include "detail/ResultConverter.h"
#include "detail/function_traits.h"

/**
 * Function ApplyCefArguments calls given function and unpacks CefListValue as arguments.
 * Argument type checks is implemented in detail::ArgumentsAdapter
 *
 * Function BindCommand transforms C++ function to the std::function
 * which accepts CefListValue and returns CefValue.
*/

using CommandCallback = std::function<std::string(const std::vector<std::string>&)>;

namespace detail
{
template<typename R, typename... Args>
R ApplyStringArguments(const std::function<R(Args...)>& function, const std::vector<std::string>& args)
{
	detail::ArgumentsAdapter adapter(args);
	detail::values_tuple<Args...> typedArgs;

	adapter.CheckArgumentsCount(std::tuple_size<decltype(typedArgs)>::value);
	detail::for_each_in_tuple(typedArgs, adapter);

	return std::apply(function, typedArgs);
}

// For non-void return values.
template<typename Result, typename... Args>
CommandCallback BindCommandImpl(const std::function<Result(Args...)>& function)
{
	return [function](const std::vector<std::string>& list) {
		return ResultConverter::Convert(ApplyStringArguments(function, list));
	};
}

// For void return values.
template<typename... Args>
CommandCallback BindCommandImpl(const std::function<void(Args...)>& function)
{
	return [function](const std::vector<std::string>& list) {
		ApplyStringArguments(function, list);
		return std::string();
	};
}
}

// For std::function, lambdas or function pointers
template<typename Callable>
CommandCallback BindCommand(Callable&& callable)
{
	return detail::BindCommandImpl(detail::make_function(callable));
}

// for pointers to const member function
template<class ClassType, class ReferenceType, class ReturnType, class... Args>
CommandCallback BindCommand(ReturnType (ClassType::*method)(Args...) const, ReferenceType&& object)
{
	using ConstClassType = typename std::add_const<ClassType>::type;

	ConstClassType* pObject = detail::make_object_pointer<ConstClassType, ReferenceType>(std::forward<ReferenceType>(object));
	return BindCommand([=](Args... args) -> ReturnType {
		return (pObject->*method)(args...);
	});
}

// for pointers to member function
template<class ClassType, class ReferenceType, class ReturnType, class... Args>
CommandCallback BindCommand(ReturnType (ClassType::*method)(Args...), ReferenceType&& object)
{
	ClassType* pObject = detail::make_object_pointer<ClassType, ReferenceType>(std::forward<ReferenceType>(object));
	return BindCommand([=](Args... args) -> ReturnType {
		return (pObject->*method)(args...);
	});
}
