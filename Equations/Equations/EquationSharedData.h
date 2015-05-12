#pragma once
#include <array>
#include <vector>
#include <map>

#include "Variable.h"

namespace EquationSharedData
{
	typedef std::vector<std::string> StringVector;
	typedef std::map<std::string, Variable> VariableMap;

	const static std::array<std::string, 3> PRIORITY_3_OPERATORS = {"*", "%", "/"};//multiplication, modulo, and division
	const static std::array<std::string, 2> PRIORITY_4_OPERATORS = {"+", "-"};//addition and subtraction
	const static std::array<const std::string, 3> WORD_OPERATORS = {"sqrt", "pow", "^"};

	struct OperatorVariables
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

