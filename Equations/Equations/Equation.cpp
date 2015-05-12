#include "Equation.h"

#include "EquationSolver.h"
#include "EquationRearranger.h"
#include "EquationParser.h"

#include <assert.h>

#include "StringExtensions.h"

Equation::Equation(void)
{
}

Equation::~Equation(void)
{
}

bool Equation::SetEquation(const std::string& equation)
{
	if(EquationParser::ValidateEquation(equation))
	{
		baseEquation = equation;
		brokenDownEquation.clear();
		variables.clear();
		brokenDownEquation = EquationParser::ExtractEquationParts(equation);
		FormatEquation();
		EquationParser::ExtractVariables(brokenDownEquation, variables);
		return true;
	}

	return false;
}

void Equation::FormatEquation()
{
	std::string eq = baseEquation;
	size_t startQuote;

	while ((startQuote = eq.find('"')) != std::string::npos)
	{
		size_t endQuote = eq.find('"', startQuote+1);

		assert(endQuote != std::string::npos);
		
		eq.erase(startQuote, endQuote - startQuote + 1);
	}

	formattedEquation = eq;
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

double Equation::GetAnswer(const std::string& varToSolve)
{
	EquationRearranger isolator = EquationRearranger(*this);

	std::vector<std::string> parts = isolator.IsolateVariable(varToSolve);
	
	EquationSolver solver = EquationSolver(parts, variables);

	return solver.Solve();
}
