// Compiler.cpp : définit le point d'entrée de l'application.
//
#include <iostream>
#include <fstream>
#include <runtime/interpreter.hpp>
#include <utils.hpp>
#include <filesystem>


void usage(const char *name)
{
	std::cout << "Usage: " << name << " <input file>" << std::endl;
}

void interprete(std::string & sourceCode, Parser * parser, Interpreter * interpreter, Environment * env)
{
	// parse the source code
	auto program = parser->produceAST(sourceCode);
	auto value = interpreter->evaluate(program, env);

	std::cout << value->toString() << std::endl;	
}

void reply(Parser * parser, Interpreter * interpreter, Environment * env)
{
	while (true) {
		std::string line;
		std::cin.clear();
		std::cout.flush();
		std::cout << ">>> ";
		std::getline(std::cin, line);
		if (line == "exit") {
			break;
		}
		if(line != "" || line != " ") {
			interprete(line, parser, interpreter, env);
		}
	}
}

void run(const std::string & path, Parser * parser, Interpreter * interpreter, Environment * env) {
	if(!std::filesystem::is_regular_file(path)) {
		throw std::runtime_error("Unable to retrieve the source file. File is not exists.");
	}

	std::string sourceCode {};

	std::ifstream input{ path };
	if (!input.is_open()) {
		std::cerr << "Error: could not open input file" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	sourceCode = std::string{ std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };
	interprete(sourceCode, parser, interpreter, env);
	input.close();
}

int main(int argc, char *argv[])
{
	
	Parser parser = Parser();
	Interpreter interpreter = Interpreter();
	Environment * env = new Environment();

	if (argc >= 2) {
		run(argv[1], &parser, &interpreter, env);
	}
	else {
		reply(&parser, &interpreter, env);
	}

	delete env;

	return 0;
}
