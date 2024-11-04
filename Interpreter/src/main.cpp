// Compiler.cpp : définit le point d'entrée de l'application.
//
#include <iostream>
#include <fstream>
#include <runtime/interpreter.hpp>
#include <utils.hpp>

void usage(const char *name)
{
	std::cout << "Usage: " << name << " <input file>" << std::endl;
}

void interprete(std::string & sourceCode) {
	Parser parser = Parser();
	Environment * env = new Environment();
	Interpreter interpreter = Interpreter();

	// add some variables in the env
	env->defineVariable("a", new NumberValue(10));

	// parse the source code
	auto program = parser.produceAST(sourceCode);
	auto value = interpreter.evaluate(program, env);

	std::cout << value->toString() << std::endl;
	
	delete value;
	delete env;
}

int main(int argc, char *argv[])
{
	
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
		interprete(codeSource);
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
			interprete(line);
		}
	}

	return 0;
}
