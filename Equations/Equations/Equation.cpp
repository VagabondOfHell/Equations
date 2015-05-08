#include "Equation.h"

#include "EquationParser.h"

#include <sstream>
#include <algorithm>

#include "StringExtensions.h"

std::array<std::string, 3> Equation::PRIORITY_3_OPERATORS = {"*", "%", "/"};

std::array<std::string, 2> Equation::PRIORITY_4_OPERATORS = {"+", "-"};

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
	else if((result = workingVariables.find(varAlias)) != workingVariables.end())
	{
		outVal = result->second;
		return true;
	}

	return false;
}

std::vector<std::string>::const_iterator Equation::GetVariablePosition( const std::vector<std::string>& vec,
	const std::vector<std::string>::const_iterator& opPos, bool lookLeft) const
{
	if(lookLeft)
	{
		if(opPos == vec.begin())
			return vec.end();

		if((opPos - 1) == vec.begin())
			return vec.begin();

		if((opPos - 2) == vec.begin())
		{
			if(*(opPos - 2) == "-")
				return opPos-2;
			else
				return opPos - 1;
		}

		if(*(opPos - 2) == "-" && (EquationParser::IsArithmeticOperator(*(opPos - 3))
			|| EquationParser::IsWordOperator(*(opPos - 3))))
			return (opPos - 2);
		else
			return (opPos - 1);
	}
	else
	{
		if(*(opPos + 1) == "-")
			return (opPos + 2);
		else
			return (opPos + 1);
	}
}

double Equation::ExecuteOperator(const double lhVal, const double rhVal, const std::string& operatorStr) const
{
	if(operatorStr == "+")
		return lhVal + rhVal;
	else if(operatorStr == "-")
		return lhVal - rhVal;
	else if(operatorStr == "*")
		return lhVal * rhVal;
	else
		return 0.0;
}

Equation::OperatorVariables Equation::GetOperatorValues(const std::vector<std::string>& portion,
			const std::vector<std::string>::const_iterator& operatorPos)const
{
	std::vector<std::string>::const_iterator clhVar = GetVariablePosition(portion, operatorPos, true);
	std::vector<std::string>::const_iterator crhVar = GetVariablePosition(portion, operatorPos, false);
	
	Variable lhVar, rhVar;
	double lhVal, rhVal;

	bool applyNegative = false;

	//Check for negative sign
	if(*clhVar == "-")
	{
		GetVariable(*(clhVar + 1), lhVar);
		applyNegative = true;
	}
	else
		GetVariable(*clhVar, lhVar);

	if(lhVar.IsUnknown())
		throw std::exception("Unknown Value During Calculation");

	if(applyNegative)
		lhVal = -lhVar.GetValue();
	else
		lhVal = lhVar.GetValue();

	applyNegative = false;

	if((operatorPos + 2) == crhVar && *(operatorPos + 1) == "-")
	{
		GetVariable(*(operatorPos + 2), rhVar);
		applyNegative = true;
	}
	else
		GetVariable(*(operatorPos + 1), rhVar);

	if(rhVar.IsUnknown())
		throw std::exception("Unknown Value During Calculation");

	if(applyNegative)
		rhVal = -rhVar.GetValue();
	else
		rhVal = rhVar.GetValue();

	return OperatorVariables(clhVar, crhVar, lhVal, rhVal);
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
	workingVariables = variables;

	std::vector<std::string> parts = IsolateVariable(varToSolve);

	unsigned int currAnswIndex = 0;

	ProcessBrackets(parts, currAnswIndex);

	StringVector portionResult = ProcessEquation(parts, currAnswIndex);

	Variable total;
	bool applyNegative = false;

	if(parts.size() == 1)
		GetVariable(parts[0], total);
	else if(parts.size() == 2)
	{
		GetVariable(parts[1], total);
		
		if(parts[0] == "-")
			applyNegative = true;
		else
			throw std::exception("Answer Too Long");
	}
	else
		throw std::exception("Unsolvable");

	if(applyNegative)
		return -total.GetValue();
	else
		return total.GetValue();
}

void Equation::ProcessBrackets(StringVector& equation, unsigned int& tlIndex)
{
	bool bracketsToSolve = hasBrackets;

	while (bracketsToSolve)
	{
		auto lastOpenBracket = std::find(equation.rbegin(), equation.rend(), "(");

		if(lastOpenBracket == equation.rend())
		{
			bracketsToSolve = false;
			break;
		}

		lastOpenBracket += 1;

		auto firstClosedBracket = std::find((lastOpenBracket.base()), equation.end(), ")");

		if(firstClosedBracket == equation.end())
			throw std::exception("Closed Bracket not found");

		auto fwdLastOpenBracket = lastOpenBracket.base();

		StringVector portionResult = 
			ProcessEquation(StringVector(fwdLastOpenBracket + 1, firstClosedBracket), tlIndex);

		for (unsigned int i = 0; i < portionResult.size(); i++)
		{
			*(fwdLastOpenBracket + i) = portionResult[i];
		}

		equation.erase(fwdLastOpenBracket + portionResult.size(), firstClosedBracket + 1);
	}
}

void Equation::ProcessOperator(StringVector& portion, unsigned int& tlIndex, StringVector::iterator& operatorIter)
{
	OperatorVariables opVars = GetOperatorValues(portion, operatorIter);

	double varVal = ExecuteOperator(opVars.lhVal, opVars.rhVal, *operatorIter);

	//convert to non-const operator in O(1) time
	std::vector<std::string>::iterator iter = portion.erase(opVars.lhValIter, opVars.lhValIter);
	*iter = "tl" + std::to_string(tlIndex);

	//Add the current working variable to the list of variables for later use
	workingVariables.insert(VariableMap::value_type(*iter, Variable(varVal, *iter)));
	++tlIndex;

	//remove the processed portion
	portion.erase(opVars.lhValIter + 1, opVars.rhValIter + 1);
}

Equation::StringVector Equation::ProcessEquation(std::vector<std::string>& portion, unsigned int& tlIndex)
{
	std::vector<std::string>::iterator beginSearch = portion.begin();

	while (auto wordOperator = EquationParser::FindFirstWordOperator(portion) != portion.end())
	{
		printf("R");
	}

	StringVector::iterator arithOp;
	beginSearch = portion.begin();

	while ((arithOp = find_first_of(portion.begin(), portion.end(), 
				PRIORITY_3_OPERATORS.begin(), PRIORITY_3_OPERATORS.end())) != portion.end())
	{
		ProcessOperator(portion, tlIndex, arithOp);

		//reset search criteria
		beginSearch = portion.begin();
	}

	beginSearch = portion.begin();

	while ((arithOp = find_first_of(beginSearch, portion.end(),
		PRIORITY_4_OPERATORS.begin(), PRIORITY_4_OPERATORS.end())) != portion.end())
	{
		//Check to see if this is a minus sign or a negative sign
		if(*arithOp == "-")
		{
			if(arithOp == portion.begin())
			{
				beginSearch = arithOp + 1;
				continue;
			}
			else if(EquationParser::IsArithmeticOperator(*(arithOp - 1)))//Also checks word operators for us
			{
				beginSearch = arithOp + 1;
				continue;
			}
		}
		
		//OperatorVariables opVars = GetOperatorValues(portion, p4Op);

		//double varVal = ExecuteOperator(opVars.lhVal, opVars.rhVal, *p4Op);

		////convert to non-const operator in O(1) time
		//std::vector<std::string>::iterator iter = portion.erase(opVars.lhValIter, opVars.lhValIter);
		//*iter = "tl" + std::to_string(tlIndex);

		////Add the current working variable to the list of variables for later use
		//workingVariables.insert(VariableMap::value_type(*iter, Variable(varVal, *iter)));
		//++tlIndex;

		////remove the processed portion
		//portion.erase(opVars.lhValIter + 1, opVars.rhValIter + 1);

		////reset search criteria
		//beginSearch = portion.begin();

		ProcessOperator(portion, tlIndex, arithOp);

		//reset search criteria
		beginSearch = portion.begin();
	}

	return portion;
}

