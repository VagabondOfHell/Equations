#pragma once
#include <string>
#include <array>
#include <vector>
#include <map>

#include "Variable.h"

class EquationParser
{
private:
	static const std::array<const std::string, 2> WORD_OPERATORS;

	static bool EvaluatePhrase(std::string& equation, const std::string& phrase);

	static bool IsWordOperator(std::string& equation, std::string& foundOperator);

	static bool InsertMultiplicationSymbol(const std::string& prevInsert);

	static bool GetBetweenSymbol(std::string& baseString, const char symbol, std::string& outVal);

public:
	typedef std::map<std::string, Variable> VariableMap;

	static bool ExtractVariables(const std::vector<std::string>& brokenDownEquation, VariableMap& mapToFill);

	static bool ValidateEquation(const std::string& equation, bool& hasBrackets);

	static std::vector<std::string> ExtractEquationParts(const std::string& equation);

	static bool IsWordOperator(const std::string& phrase);

	static bool IsArithmeticOperator(const std::string& phrase);

	static std::vector<std::string>::const_iterator FindFirstWordOperator(const std::vector<std::string>& vecToSearch);
};

