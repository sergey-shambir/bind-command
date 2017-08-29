#pragma once
#include <system_error>
#include <utility>

// TODO: remove std hack when libc++ will implement from_chars.
namespace std
{
using from_chars_result = std::pair<const char*, std::error_code>;

inline from_chars_result from_chars(const char* begin, const char* end, float& value)
{
	from_chars_result res;
	char* endptr = nullptr;
	value = strtof(begin, &endptr);
	res.first = endptr;
	if (endptr && (*endptr != 0))
	{
		res.second = std::error_code(EDOM, std::system_category());
	}

	return res;
}

inline from_chars_result from_chars(const char* begin, const char* end, double& value)
{
	from_chars_result res;
	char* endptr = nullptr;
	value = strtod(begin, &endptr);
	res.first = endptr;
	if (endptr && (*endptr != 0))
	{
		res.second = std::error_code(EDOM, std::system_category());
	}

	return res;
}

inline from_chars_result from_chars(const char* begin, const char* end, int& value)
{
	from_chars_result res;
	char* endptr = nullptr;
	value = static_cast<int>(strtol(begin, &endptr, 10));
	res.first = endptr;
	if (endptr && (*endptr != 0))
	{
		res.second = std::error_code(EDOM, std::system_category());
	}

	return res;
}

inline from_chars_result from_chars(const char* begin, const char* end, unsigned& value)
{
	from_chars_result res;
	char* endptr = nullptr;
	value = static_cast<unsigned>(strtol(begin, &endptr, 10));
	res.first = endptr;
	if (endptr && (*endptr != 0))
	{
		res.second = std::error_code(EDOM, std::system_category());
	}

	return res;
}
}
