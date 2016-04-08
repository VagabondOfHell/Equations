#pragma once
#include "EquationSharedData.h"

class EquationParser
{
private:
	//Check if the phrase can be found within the equation
	static bool EvaluatePhrase(std::string& equation, const std::string& phrase);
	//Checks if the text is a word operator, and returns it if it is (Sin, Cos, etc)
	static bool IsWordOperator(std::string& equation, std::string& foundOperator);
	//Checks if a multiplication symbol should be inserted at the current position
	static bool InsertMultiplicationSymbol(const std::string& prevInsert);
	//Get what is between two instances of the specified symbol
	static bool GetBetweenSymbol(std::string& baseString, const char symbol, std::string& outVal);
	//Get a string representation of a number
	static bool GetNumberString(std::string& baseString, std::string& outVal);

public:
	//TODO: In V2, look at making this private, and doing it while parsing the equation
	//Pulls out variables from a parsed equation and copies to a map of variables for easy reference
	static bool ExtractVariables(std::vector<std::string>& brokenDownEquation, EquationSharedData::VariableMap& mapToFill);
	//Ensure that the equation has the proper components (equal sign, all brackets are closed, etc.)
	static bool ValidateEquation(const std::string& equation);
	//Breaks a raw string equation into its individual parts
	static std::vector<std::string> ExtractEquationParts(const std::string& equation);
	//Checks if the specified phrase is a word operator (Sin, Cos, etc.)
	static bool IsWordOperator(const std::string& phrase);
	//Checks if the specified phrase is an arithmetic operator (+, -, /, = etc)
	static bool IsArithmeticOperator(const std::string& phrase);
	//Finds the first word operator in the list
	static std::vector<std::string>::const_iterator FindFirstWordOperator(const std::vector<std::string>& vecToSearch);
};

