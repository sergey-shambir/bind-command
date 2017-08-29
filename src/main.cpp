#include "BindCommand.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <map>

using namespace std;

using Command = function<string(const vector<string>&)>;

map<string, Command> BuildCommandMap()
{
	return {
		{ "hello", BindCommand([](const std::string& name) {
			 std::cout << "Hello, " << name << std::endl;
		 }) },
		{ "sum", BindCommand([](int a, int b) {
			 return a + b;
		 }) },
		{ "exit", BindCommand([]() {
			 exit(0);
		 }) },
	};
}

pair<string, vector<string>> SplitCommandAndArgs(const string& str)
{
	string commandName;
	vector<string> arguments;

	boost::split(arguments, str, boost::is_space(), boost::token_compress_on);
	if (!arguments.empty())
	{
		commandName = arguments.front();
		arguments.erase(arguments.begin());
	}

	return { commandName, arguments };
}

int main()
{
	auto commands = BuildCommandMap();

	std::string input;
	while (std::getline(std::cin, input))
	{
		auto[commandName, args] = SplitCommandAndArgs(input);
		if (commandName.empty())
		{
			continue;
		}
		auto commandIt = commands.find(commandName);
		if (commandIt == commands.end())
		{
			cerr << "invalid command: " << commandName << endl;
			continue;
		}

		try
		{
			const string result = commandIt->second(args);
			if (!result.empty())
			{
				cout << "result: " << result << endl;
			}
		}
		catch (const std::exception& ex)
		{
			cerr << "Call failed: " << ex.what() << endl;
		}
	}
}
