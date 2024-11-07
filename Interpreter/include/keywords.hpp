#pragma once

#include <vector>
#include <string>


const std::vector<std::string> keywords = {
	/* var types */
	"number", 
	"string",
	"bool", 
	/* null type|value */
	"null",
	"true",
	"false",
	/* conditional expressions */
	"if", 
	"else",
	/* logic operators */
	"and", "or", "not",
	 /* loops */
	"for"
};

const std::vector<std::string> keywordsLitterals = {
	"null",
	"true",
	"false"
};

const std::vector<std::string> variablesKeywords = {
	"bool",
	"number",
	"string"
};

const std::vector<std::string> conditionalKeywords = {
	"if",
	"else"
};

const std::vector<std::string> logicExpressionKeywords = {
	"not", "and", "or"
};
