#pragma once
#include "EquationSharedData.h"

class EquationParser
{
private:

	static bool EvaluatePhrase(std::string& equation, const std::string& phrase);

	static bool IsWordOperator(std::string& equation, std::string& foundOperator);

	static bool InsertMultiplicationSymbol(const std::string& prevInsert);

	static bool GetBetweenSymbol(std::string& baseString, const char symbol, std::string& outVal);

	static bool GetNumberString(std::string& baseString, std::string& outVal);

public:
	static bool ExtractVariables(std::vector<std::string>& brokenDownEquation, EquationSharedData::VariableMap& mapToFill);

	static bool ValidateEquation(const std::string& equation);

	static std::vector<std::string> ExtractEquationParts(const std::string& equation);

	static bool IsWordOperator(const std::string& phrase);

	static bool IsArithmeticOperator(const std::string& phrase);

	static std::vector<std::string>::const_iterator FindFirstWordOperator(const std::vector<std::string>& vecToSearch);
};

