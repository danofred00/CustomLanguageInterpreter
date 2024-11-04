// Compiler.cpp : définit le point d'entrée de l'application.
//
#include <iostream>
#include <fstream>
#include <parser.hpp>
#include <utils.hpp>

void usage(const char *name)
{
	std::cout << "Usage: " << name << " <input file>" << std::endl;
}

int main(int argc, char *argv[])
{
	Parser parser = Parser();
	std::string codeSource{};

	if (argc >= 2)
	{
		std::ifstream input{ argv[1] };
		if (!input.is_open())
		{
			std::cerr << "Error: could not open input file" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		codeSource = std::string{ std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };
		auto program = parser.produceAST(codeSource);
		std::cout << program->toString() << std::endl;
		input.close();
	}
	else {
		while (true) {
			std::string line;
			std::cout << ">>> ";
			std::getline(std::cin, line);
			if (line == "exit") {
				break;
			}
			auto program = parser.produceAST(line);
			std::cout << program->toString() << std::endl;
		}
	}

	return 0;
}
