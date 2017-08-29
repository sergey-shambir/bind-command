#pragma once

#include <functional>
#include <utility>

/**
* Function BindCommand transforms C++ function to the std::function
* which accepts `vector<string>` and returns `string>.
*/

namespace detail
{
/////////////////////////////////////////////////////////////////////////////////////////////////
// Tuple utilities: foreach and values tuple

// Calls `function` for each index in integer sequence `Is`
template<typename Tuple, typename Function, size_t... Is>
void for_each_in_tuple_impl(
	Tuple&& args,
	Function const& function,
	std::integer_sequence<size_t, Is...>)
{
	(..., function(std::get<Is>(args), Is));
}

// Calls `function` for each tuple element
template<typename... Ts, typename Function>
void for_each_in_tuple(std::tuple<Ts...>& params, Function const& function)
{
	for_each_in_tuple_impl(params, function, std::index_sequence_for<Ts...>());
}

// Remove `const&` and other dangerous qualifiers.
template<typename... Args>
using values_tuple = std::tuple<typename std::decay<Args>::type...>;

/////////////////////////////////////////////////////////////////////////////////////////////////
// Convert std::reference_wrapper<T>, T, T* or some_pointer<T> to T*
// Const qualifier stays preserved.

struct tag_dereferenced_address
{
};
struct tag_raw_address
{
};

template<class ClassType, class ReferenceType>
inline ClassType* make_object_pointer_impl(ReferenceType&& reference, tag_dereferenced_address)
{
	return &(*reference);
}

template<class ClassType, class ReferenceType>
inline ClassType* make_object_pointer_impl(ReferenceType&& reference, tag_raw_address)
{
	return &reference;
}

template<class ClassType, class ReferenceType>
inline ClassType* make_object_pointer(ReferenceType&& reference)
{
	using RawClassType = typename std::decay_t<ClassType>;
	using RawReferenceType = typename std::decay_t<ReferenceType>;
	using TagType = typename std::conditional_t<
		std::is_same_v<RawClassType, RawReferenceType>,
		tag_raw_address,
		tag_dereferenced_address>;

	return make_object_pointer_impl<ClassType, ReferenceType>(std::forward<ReferenceType>(reference), TagType());
}

} // namespace detail
