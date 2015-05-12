#pragma once
#include "EquationSharedData.h"

class Equation;

class EquationRearranger
{
private:
	std::vector<std::string> workingEquation;
	const Equation& equation;

	void RearrangeEquation(const std::vector<std::string>::const_iterator& variablePos,
		std::vector<std::string>& outVal);

	std::string InvertOperator(const std::string& operatorVal);

public:
	EquationRearranger(const Equation& equation);
	~EquationRearranger();

	std::vector<std::string> IsolateVariable(const std::string& variable);
};

