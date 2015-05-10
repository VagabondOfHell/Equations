#include "Equation.h"

#include "EquationSolver.h"

#include "EquationParser.h"

#include <sstream>
#include <algorithm>

#include "StringExtensions.h"

Equation::Equation(void)
	:hasBrackets(false)
{
}

Equation::~Equation(void)
{
}

bool Equation::SetEquation(const std::string& equation)
{
	if(EquationParser::ValidateEquation(equation, hasBrackets))
	{
		baseEquation = equation;
		brokenDownEquation.clear();
		variables.clear();
		brokenDownEquation = EquationParser::ExtractEquationParts(equation);
		EquationParser::ExtractVariables(brokenDownEquation, variables);
		return true;
	}

	return false;
}

bool Equation::SetVariable(const std::string& varAlias, double varVal)
{
	VariableMap::iterator result = variables.find(varAlias);

	if(result != variables.end())
	{
		result->second.SetValue(varVal);
		return true;
	}

	return false;
}

bool Equation::GetVariable(const std::string& varAlias, Variable& outVal)const
{
	VariableMap::const_iterator result = variables.find(varAlias);

	if(result != variables.end())
	{
		outVal = result->second;
		return true;
	}

	return false;
}

Equation::StringVector Equation::IsolateVariable(const std::string& variable) const
{
	StringVector parts = brokenDownEquation;

	auto equalIter = std::find(parts.begin(), parts.end(), "=");
	auto varIter = std::find(parts.begin(), parts.end(), variable);

	//need to add support for rearranging the equation here



	if(equalIter == parts.end() || varIter == parts.end())
		throw std::exception("Equal Sign or Desired Variable Not Found");

	int iterDiff = equalIter - varIter;//distance between equal sign and variable

	if((iterDiff == 1 || iterDiff == -1) && //Check that the variable is beside the equal sign and
		(varIter - parts.begin() == 0 || parts.end() - varIter == 1)) //that it is at the very end or very beginning
	{
		//erase the variable and equal sign from the equation, so we only process what's needed
		if(equalIter > varIter)
			parts.erase(parts.begin(), equalIter + 1);
		else
			parts.erase(equalIter, parts.end());
	}

	return parts;
}

double Equation::GetAnswer(const std::string& varToSolve)
{
	std::vector<std::string> parts = IsolateVariable(varToSolve);
	
	EquationSolver solver = EquationSolver(parts, variables);

	return solver.Solve();
}
