#pragma once
#include <array>
#include <vector>
#include <map>

#include "Variable.h"

namespace EquationSharedData
{
	//Code shared between the Equation, Solver, Parser, and Rearranger classes
	typedef std::vector<std::string> StringVector;
	typedef std::map<std::string, Variable> VariableMap;
	
	//Priorities of operators, with Brackets and Exponents taking Priority 1 and 2
	const static std::array<std::string, 3> PRIORITY_3_OPERATORS = {"*", "%", "/"};//multiplication, modulo, and division
	const static std::array<std::string, 2> PRIORITY_4_OPERATORS = {"+", "-"};//addition and subtraction
	const static std::array<const std::string, 3> WORD_OPERATORS = {"sqrt", "pow", "^"}; //Add trig functions and log in V2

	struct OperatorVariables //For the Equation Solver primarily in the processing of operators from String to their functional equivalent
	{
		std::vector<std::string>::const_iterator lhValIter;
		std::vector<std::string>::const_iterator rhValIter;
		double lhVal;
		double rhVal;

		bool leftValid, rightValid;

		OperatorVariables()
			:lhVal(0), rhVal(0)
		{

		}

		OperatorVariables(std::vector<std::string>::const_iterator lhIter, 
			std::vector<std::string>::const_iterator rhIter, double leftVal, double rightVal,
			bool lhValid = true, bool rhValid = true)
			: lhValIter(lhIter), rhValIter(rhIter), lhVal(leftVal), rhVal(rightVal), 
			leftValid(lhValid), rightValid(rhValid)
		{

		}
	};
};

