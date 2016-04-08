#include "EquationSolver.h"

#include "EquationParser.h"

#include <assert.h>

using namespace EquationSharedData;

EquationSolver::EquationSolver()
	:tempValIndex(0)
{

}

EquationSolver::EquationSolver(const std::vector<std::string>& brokenDownEquation, 
			const std::map<std::string, Variable>& extractedVariables)
			:tempValIndex(0), workingEquation(brokenDownEquation), workingVariables(extractedVariables)
{

}

EquationSolver::~EquationSolver()
{
	
}

double EquationSolver::Solve()
{
	//Process bracketed sections first. This calls process equation for each set of brackets. 
	//It could be placed within process equation, but having it here makes it cleaner and avoids recursion in my opinion
	ProcessBrackets(workingEquation);
	StringVector portionResult = ProcessEquation(workingEquation);

	Variable total;
	bool applyNegative = false;

	//If the size of our equation is now 1, we have our answer and we just need to get the variable value
	if(workingEquation.size() == 1)
		GetVariable(workingEquation[0], total);
	else if(workingEquation.size() == 2) 
	{
		//If the size is 2, check for a negative sign, otherwise we have an issue
		GetVariable(workingEquation[1], total);

		if(workingEquation[0] == "-")
			applyNegative = true;
		else
			throw std::exception("Answer Too Long");
	}
	else//If we reach this point after processing the equation, we don't know how else to proceed
		throw std::exception("Unsolvable");

	//Apply the negative sign if necessary
	if(applyNegative)
		return -total.GetValue();
	else
		return total.GetValue();
}

void EquationSolver::ProcessBrackets(StringVector& equation)
{
	StringVector::reverse_iterator lastOpenBracket;

	while ((lastOpenBracket = std::find(equation.rbegin(), equation.rend(), "(")) != equation.rend())
	{
		lastOpenBracket += 1;//move one past the bracket and begin search for closed bracket there
		auto firstClosedBracket = std::find((lastOpenBracket.base()), equation.end(), ")");

		assert(firstClosedBracket != equation.end());//this should never happen unless Equation Validation was skipped

		auto fwdLastOpenBracket = lastOpenBracket.base();//convert to fwd iterator

		//Process the section between the two brackets
		StringVector portionResult = ProcessEquation(StringVector(fwdLastOpenBracket + 1, firstClosedBracket));

		//iterate from the open bracket and convert to the result. There could be multiples
		//in the case of negative answers (the negative sign stays while the answer is input one slot after)
		for (unsigned int i = 0; i < portionResult.size(); i++)
		{
			*(fwdLastOpenBracket + i) = portionResult[i];
		}

		//remove any slots not used for the answers
		equation.erase(fwdLastOpenBracket + portionResult.size(), firstClosedBracket + 1);
	}
}

StringVector EquationSolver::ProcessEquation(StringVector& portion)
{
	StringVector::iterator arithOp, beginSearch = portion.begin();
	StringVector::const_iterator wordOp;
	
	//Solve all 2nd tier operators (sqrt, ^, cos, other word operators)
	while ((wordOp = EquationParser::FindFirstWordOperator(portion)) != portion.end())
	{
		//StringVector::iterator nonConstIter = portion.erase(wordOp, wordOp);//constant time conversion from const iterator
		ProcessOperator(portion, wordOp);
	}

	//Solve all 3rd tier operators (Multiplication, Division, Modulo, etc.)
	while ((arithOp = find_first_of(portion.begin(), portion.end(), 
				PRIORITY_3_OPERATORS.begin(), PRIORITY_3_OPERATORS.end())) != portion.end())
	{
		ProcessOperator(portion, arithOp);
	}

	//Solve all 4th tier operators (Subtraction, Negative Sign, and Addition)
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
		
		ProcessOperator(portion, arithOp);

		//reset search criteria
		beginSearch = portion.begin();
	}

	return portion;
}

void EquationSolver::ProcessOperator(StringVector& portion, const StringVector::const_iterator& operatorIter)
{
	//Get the left and right variables (or only right in some cases i.e. sqrt)
	OperatorVariables opVars = GetOperatorValues(portion, operatorIter);

	//Get the result of the operator
	double varVal = ExecuteOperator(opVars.lhVal, opVars.rhVal, *operatorIter);

	//convert to non-const operator in O(1) time
	std::vector<std::string>::iterator iter = portion.erase(opVars.lhValIter, opVars.lhValIter);
	
	//create placeholder in the string vector containing the answer of this operator and increment index
	*iter = "tl" + std::to_string(tempValIndex);
	++tempValIndex;
	
	//Add the current working variable to the list of variables for later use
	workingVariables.insert(VariableMap::value_type(*iter, Variable(varVal, *iter)));
	
	//remove the processed portion
	portion.erase(opVars.lhValIter + 1, opVars.rhValIter + 1);
}

OperatorVariables EquationSolver::GetOperatorValues(const StringVector& portion,
		const StringVector::const_iterator& operatorPos)const
{
	//Get the iterators representing beginning of variable for left, and end of variable for right
	//(allows access to negative signs)
	StringVector::const_iterator clhVar = GetVariablePosition(portion, operatorPos, true);
	StringVector::const_iterator crhVar = GetVariablePosition(portion, operatorPos, false);

	Variable lhVar, rhVar;
	double lhVal, rhVal; lhVal = rhVal = 0.0;

	bool applyNegative = false, leftValid, rightValid;  leftValid = rightValid = true;

	if(clhVar == portion.end())
		leftValid = false;

	if(crhVar == portion.end())
		rightValid = false;

	//Check that left is valid and the operator isn't square root
	//(if it is square root, we ignore the left value, because we only need the right value for it)
	if(leftValid && *operatorPos != "sqrt")
	{
		if(*clhVar == "-")
		{
			//We assert these because these should never happen. Proper verification of the equation and
			//code that returns expected results should mean that at this point, this should never be false)
			assert(GetVariable(*(clhVar + 1), lhVar));
			applyNegative = true;
		}
		else
			assert(GetVariable(*clhVar, lhVar));

		//Check if value should be negated at this point
		lhVal = applyNegative ? -lhVar.GetValue() : lhVar.GetValue();
	}
	else
	{
		//Meant mostly if the operator is the square root operator, invalidate the left value and set it's
		//position to the operator position so that when this method returns its iterators, unprocessed
		//variables don't get removed
		leftValid = false;
		clhVar = operatorPos;
	}

	applyNegative = false;//reset apply negative

	if(rightValid)
	{
		//Check if we should apply a negative
		if((operatorPos + 2) == crhVar && *(operatorPos + 1) == "-")
		{
			assert(GetVariable(*(operatorPos + 2), rhVar));
			applyNegative = true;
		}
		else//otherwise just get the variable
			assert(GetVariable(*(operatorPos + 1), rhVar));

		rhVal = applyNegative ? -rhVar.GetValue() : rhVar.GetValue();
	}

	if(lhVar.IsUnknown() || rhVar.IsUnknown())//Throw an exception if the left or right variable wasn't set
		throw std::exception("Unknown Value During Calculation");

	return OperatorVariables(clhVar, crhVar, lhVal, rhVal, leftValid, rightValid);
}

StringVector::const_iterator EquationSolver::GetVariablePosition(const std::vector<std::string>& vec,
			 const StringVector::const_iterator& opPos, bool lookLeft) const
{
	if(lookLeft)//if looking to the left of the provided operator position
	{
		if(opPos == vec.begin())//check if the operator is the beginning
			return vec.end();//and return an invalid iterator

		if((opPos - 1) == vec.begin())//Check if moving one over puts us at the beginning
			return vec.begin();//return the beginning, as that should be our variable

		if((opPos - 2) == vec.begin())//if there are two slots from the beginning
		{
			if(*(opPos - 2) == "-")//check if it is a negative sign, and if so return that
				return opPos-2;
			else
				return opPos - 1;//otherwise return one variable from the operator
		}

		//if we reach here, it means we are at least 3 slots from the beginning, so we check
		//for negative sign and to make sure that the 3rd slot is an operator of some kind,
		//otherwise this may be a subtraction sign, not a negative sign
		if(*(opPos - 2) == "-" && EquationParser::IsArithmeticOperator(*(opPos - 3)))
			return (opPos - 2);
		else
			return (opPos - 1);
	}
	else//if we are looking to the right of the operator, it becomes much easier
	{
		//if the next slot is a minus sign, we move over an extra slot and grab that
		if(*(opPos + 1) == "-")
			return (opPos + 2);
		else
			return (opPos + 1);//otherwise we just return one slot further. 
		//Should check for vector end here too, but so far no problems
	}
}

double EquationSolver::ExecuteOperator(const double lhVal, const double rhVal, const std::string& operatorStr) const
{
	//perform the proper operations based on the operator string provided and the two values surrounding it
	if(operatorStr == "+")
		return lhVal + rhVal;
	else if(operatorStr == "-")
		return lhVal - rhVal;
	else if(operatorStr == "*")
		return lhVal * rhVal;
	else if(operatorStr == "/")
		return lhVal / rhVal;
	else if(operatorStr == "%")
		return (int)lhVal % (int)rhVal;
	else if(operatorStr == "sqrt")
		return sqrt(rhVal);
	else if(operatorStr == "pow" || operatorStr == "^")
		return pow(lhVal, rhVal);
	else
		return 0.0;
}
