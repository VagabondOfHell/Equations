#pragma once
#include "Variable.h"

#include <map>
#include <vector>
#include <array>

class Equation
{
private:
	typedef std::map<std::string, Variable> VariableMap;
	typedef std::vector<std::string> StringVector;

	bool hasBrackets;
	
	///<summary>Checks to see if the passed equation is formed correctly</summary>
	///<param name="equation">The string representing the equation</param>
	///<returns>True if valid, false if not</returns>
	bool ValidateEquation(const std::string& equation)const;

protected:

	StringVector IsolateVariable(const std::string& variable)const;

	StringVector brokenDownEquation;
	VariableMap variables;

	std::string baseEquation;

public:
	Equation(void);
	virtual ~Equation(void);

	bool SetEquation(const std::string& equation);

	bool SetVariable(const std::string& varAlias, double varVal);

	bool GetVariable(const std::string& varAlias, Variable& outVal)const;

	double GetAnswer(const std::string& varToSolve);
};

