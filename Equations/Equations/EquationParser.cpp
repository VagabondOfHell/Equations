#include "EquationParser.h"
#include "StringExtensions.h"

const std::array<const std::string, 2> EquationParser::WORD_OPERATORS = {"sqrt", "pow"};

bool EquationParser::ValidateEquation(const std::string& equation, bool& hasBrackets)
{
	int numBrackets = 0;
	int numSingleQuotes = 0;
	int numDoubleQuotes = 0;
	int numEqualSigns = 0;

	bool setHasBrackets = false;

	for (unsigned int i = 0; i < equation.size(); i++)
	{
		switch (equation[i])
		{
		case '(':
			numBrackets++;
			setHasBrackets = true;
			break;
		case ')':
			numBrackets--;
			break;
		case '\'':
			numSingleQuotes++;
			break;
		case '"':
			numDoubleQuotes++;
			break;
		case '=':
			numEqualSigns++;
			break;
		default:
			break;
		}
	}

	if(numBrackets != 0 || numEqualSigns > 1 || numSingleQuotes % 2 != 0 || numDoubleQuotes % 2 != 0)
	{
		return false;
	}
	else
	{
		hasBrackets = setHasBrackets;
		return true;
	}
}

std::vector<std::string> EquationParser::ExtractEquationParts(const std::string& equation)
{
	std::string copyVal = equation;

	std::vector<std::string> retVal;

	while (!copyVal.empty())
	{
		if(StringExtensions::IsSpace(copyVal[0]))
			copyVal.erase(0, 1);
		else if(StringExtensions::IsLetter(copyVal[0]))
		{
			std::string wordOperator;

			if(retVal.size() > 0 && InsertMultiplicationSymbol(retVal[retVal.size() - 1]))
				retVal.push_back("*");

			if(!IsWordOperator(copyVal, wordOperator))
			{
				std::string temp; temp += copyVal[0];
				retVal.push_back(temp);
				copyVal.erase(0, 1);
			}
			else
				retVal.push_back(wordOperator);
		}
		else if(StringExtensions::IsPunctuation(copyVal[0]))
		{
			std::string outVal;

			if(copyVal[0] == '\'' && GetBetweenSymbol(copyVal, '\'', outVal))
			{
				retVal.push_back(outVal);
			}
			else if(copyVal[0] == '"' && GetBetweenSymbol(copyVal, '"', outVal))
			{
				//don't do anything, because the GetBetweenSymbol removes it for us
			}
			else
			{
				std::string temp; temp += copyVal[0];

				if(temp == "(" && retVal.size() > 0 && InsertMultiplicationSymbol(retVal[retVal.size() - 1]))
					retVal.push_back("*");

				retVal.push_back(temp);
				copyVal.erase(0, 1);
			}
		}
	}

	return retVal;
}

bool EquationParser::InsertMultiplicationSymbol(const std::string& prevInsert)
{
	//Check to see if we should insert a multiplication symbol before this variable
	//i.e. y = mx + b will be treated y = m*x+b.    5(x-b) will be 5 * (x-b)
	if(!StringExtensions::IsAllAlphanumeric(prevInsert) &&	prevInsert != ")")
		return false;

	if(IsWordOperator(prevInsert))
		return false;

	return true;
}

bool EquationParser::IsArithmeticOperator(const std::string& phrase)
{
	if(phrase.size() == 1)
	{
		if(phrase[0] == '*' || phrase[0] == '/' || phrase[0] == '+' ||
			phrase[0] == '-' || phrase[0] == '%' || phrase[0] == '=')
			return true;
	}

	if(phrase.size() > 1 && IsWordOperator(phrase))
		return true;

	return false;
}

std::vector<std::string>::const_iterator 
	EquationParser::FindFirstWordOperator(const std::vector<std::string>& vecToSearch)
{
	return std::find_first_of(vecToSearch.begin(), vecToSearch.end(),
		WORD_OPERATORS.begin(), WORD_OPERATORS.end());
}

bool EquationParser::ExtractVariables(const std::vector<std::string>& brokenDownEquation, VariableMap& mapToFill)
{
	Variable* lastVariableCreated = NULL;

	for (unsigned int i = 0; i < brokenDownEquation.size(); i++)
	{
		if(brokenDownEquation[i][0] == '"')
		{
			if(lastVariableCreated)
				lastVariableCreated->Name = StringExtensions::RemoveAllOfSymbol(brokenDownEquation[i], '"');
		}
		else if(StringExtensions::IsAllAlphanumeric(brokenDownEquation[i]) && !IsWordOperator(brokenDownEquation[i]))
		{
			lastVariableCreated = &mapToFill.insert
				(VariableMap::value_type(brokenDownEquation[i], Variable(brokenDownEquation[i]))).first->second;
		}
	}

	return true;
}

bool EquationParser::EvaluatePhrase(std::string& equation, const std::string& phrase)
{
	std::string testString = equation.substr(0, phrase.size());

	if(testString.find(phrase.c_str(), 0, phrase.size()) != std::string::npos)
	{
		equation.erase(0, phrase.size());
		return true;
	}

	return false;
}

bool EquationParser::IsWordOperator(std::string& equation, std::string& foundOperator)
{
	for (unsigned int i = 0; i < WORD_OPERATORS.size(); i++)
	{
		if(EvaluatePhrase(equation, WORD_OPERATORS[i]))
		{
			foundOperator = WORD_OPERATORS[i];
			return true;
		}
	}

	return false;
}

bool EquationParser::IsWordOperator(const std::string& phrase)
{
	for (unsigned int i = 0; i < WORD_OPERATORS.size(); i++)
	{
		if(StringExtensions::FindToPoint(phrase, WORD_OPERATORS[i], WORD_OPERATORS[i].size()))
			return true;
	}

	return false;
}

bool EquationParser::GetBetweenSymbol(std::string& baseString, const char symbol, std::string& outVal)
{
	std::string::size_type firstSymbol = baseString.find_first_of(symbol);

	if(firstSymbol == std::string::npos)
		return false;

	std::string::size_type secondSymbol = baseString.find_first_of(symbol, firstSymbol + 1);

	if(secondSymbol == std::string::npos)
		return false;

	std::string section = baseString.substr(firstSymbol, secondSymbol);
	baseString.erase(firstSymbol, secondSymbol+1);
	section = StringExtensions::RemoveAllOfSymbol(section, symbol);
	outVal = section;
	return true;
}
