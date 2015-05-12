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
	
	///<summary>Checks to see if the passed equation is formed correctly</summary>
	///<param name="equation">The string representing the equation</param>
	///<returns>True if valid, false if not</returns>
	bool ValidateEquation(const std::string& equation)const;

	///<summary>Takes the base equation and removes all double quotes to show it in its normal form</summary>
	void FormatEquation();

protected:
	friend class EquationRearranger;

	StringVector brokenDownEquation;
	VariableMap variables;

	std::string baseEquation;
	std::string formattedEquation;

public:
	Equation(void);
	virtual ~Equation(void);

	bool SetEquation(const std::string& equation);

	inline std::string GetEquation()const{return baseEquation;}
	inline std::string GetFormattedEquation()const {return formattedEquation;}

	bool SetVariable(const std::string& varAlias, double varVal);

	bool GetVariable(const std::string& varAlias, Variable& outVal)const;

	double GetAnswer(const std::string& varToSolve);
};

