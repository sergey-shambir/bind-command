#include "BindCommand.h"
#include <iostream>

using namespace std::literals;

int main()
{
	auto command = BindCommand([](const std::string& name) {
		std::cout << "Hello, " << name << std::endl;
	});

	command(std::vector{ "Vasya"s });

	auto command2 = BindCommand([](int a, int b) {
		return a + b;
	});

	std::string result = command2(std::vector{ "1941"s, "4"s });
	std::cout << result << std::endl;
}
