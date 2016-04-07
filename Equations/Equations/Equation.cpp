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
		//Store the raw string format of the equation
		baseEquation = equation;
		//If we are being re-used, make sure there are no left overs from previous equation
		brokenDownEquation.clear();
		variables.clear();
		//Separate the portions of the equation into separate strings contained in a vector
		brokenDownEquation = EquationParser::ExtractEquationParts(equation);
		//Remove variable names from equation and store the result in FormattedEquation
		FormatEquation();
		//Extract the string portions that pertain to variables from the list we filled above
		EquationParser::ExtractVariables(brokenDownEquation, variables);
		return true;
	}

	return false;
}

void Equation::FormatEquation()
{
	std::string eq = baseEquation;
	size_t startQuote;
	
	//Continuously find double quotes in the provided equation
	while ((startQuote = eq.find('"')) != std::string::npos)
	{
		//Find the next one that will be considered the closing quotes
		size_t endQuote = eq.find('"', startQuote+1);
		//Remove everything between them. There will always be a closing quote at this point because
		//if there isn't, ValidateEquation would fail.
		eq.erase(startQuote, endQuote - startQuote + 1);
	}
	//Set the formatted equation to the newly modified version
	formattedEquation = eq;
}

///Set the value of a variable that is found in the equation. If an incorrect alias is passed, this function fails.
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

//TODO: Set Variable by name

///Returns the variable in the outval parameter if successfully found. Should convert this to return a pair instead
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
	//These 2 lines do nothing right now. They are used for Version 2, when we can re-arrange an equation
	//to solve for any variable
	EquationRearranger isolator = EquationRearranger(*this);
	std::vector<std::string> parts = isolator.IsolateVariable(varToSolve);
	
	//Create a solver with this equations data
	EquationSolver solver = EquationSolver(parts, variables);

	//The meat and potatoes are in this guy right here
	return solver.Solve();
}
