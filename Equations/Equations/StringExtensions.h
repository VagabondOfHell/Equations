#pragma once
#include <string>
#include <cctype>

#include <vector>
#include <algorithm>

class StringExtensions
{
public:
	static std::vector<std::string> ParseAllWhiteSpace(const std::string& stringToParse);

	static inline bool IsSpace(const char c)
	{
		return std::isspace(c) != 0;
	}

	static inline bool IsLetter(const char c)
	{
		return std::isalpha(c) != 0;
	}

	///<summary>Checks if the given character is part of a specific list </summary>
	///<param name="c">The character to check</param>
	///<param name="oneOfList">The list of characters to compare against</param>
	///<returns>True if found, false if not</returns>
	static inline bool IsOneOf(const char c, const std::string& oneOfList)
	{
		return oneOfList.find(c) != std::string::npos;
	}

	static inline bool IsAllLetters(const std::string& str)
	{
		for (unsigned int i = 0; i < str.length(); i++)
		{
			if(!std::isalpha(str[i]))
				return false;
		}

		return true;
	}

	static inline bool IsNumber(const char c)
	{
		return std::isdigit(c) != 0;
	}

	///<summary>Checks if the passed string is all numbers</summary>
	///<param name="str">The string to evaluate</param>
	///<param name="checkDecimal">True to allow checking of decimal places. Will not allow multiple decimal
	///points in the same section</param>
	///<param name="allowWhiteSpace">True to allow white space in the string. Useful for checking
	///multiple number entries in the same string </param>
	///<returns>True if all numbers, false if not. False if more than one decimal is found
	///within a number sequence</returns>
	static inline bool IsAllNumbers(const std::string& str, bool checkDecimal = false, bool allowWhiteSpace = false)
	{
		bool decimalFound = false;

		for (unsigned int i = 0; i < str.size(); i++)
		{
			if(!std::isdigit(str[i]))
			{
				if(checkDecimal && !decimalFound && str[i] == '.')
				{
					decimalFound = true;
					continue;
				}

				if(allowWhiteSpace && std::isspace(str[i]))
				{
					//reset decimal found because we are working with a new length of numbers
					decimalFound = false;
					continue;
					
				}

				return false;
			}
		}

		return true;
	}

	static inline bool IsAlphanumeric(const char c)
	{
		return std::isalnum(c) != 0;
	}

	static inline bool IsAllAlphanumeric(const std::string& str)
	{
		for (unsigned int i = 0; i < str.size(); i++)
		{
			if(!IsAlphanumeric(str[i]))
				return false;
		}

		return true;
	}

	///<summary>Checks for any kind of graphic essentially (punctuation, digit, or letter)</summary>
	///<param name="c">The character to evaluate</param>
	///<returns>True if graphic, false if not</returns>
	static inline bool IsGraphic(const char c)
	{
		return std::isgraph(c) != 0;
	}

	///<summary>Checks for any kind of punctionation (brackets, arithmetic operators, etc)</summary>
	///<param name="c">The character to evaluate</param>
	///<returns>True if punctuation, false if not</returns>
	static inline bool IsPunctuation(const char c)
	{
		return std::ispunct(c) != 0;
	}

	static inline std::string RemoveAllOfSymbol(const std::string& baseString, const char symbol)
	{
		std::string copyVal = baseString;
		copyVal.erase(std::remove(copyVal.begin(), copyVal.end(), symbol));
		return copyVal;
	}

	static inline std::string RemoveAllOfSymbol(const std::string& baseString, const std::string& symbol)
	{
		std::string copyVal = baseString;
		std::string::size_type foundPos;

		while ((foundPos = copyVal.find(symbol)) != std::string::npos)
		{
			copyVal.erase(foundPos, symbol.size());
		}

		return copyVal;
	}

	static inline bool FindToPoint(const std::string& basePhrase, 
		const std::string& phraseToFind, unsigned int length)
	{
		std::string testString = basePhrase.substr(0, length);

		return (testString.find(phraseToFind.c_str(), 0, phraseToFind.size()) != std::string::npos);
	}
	
};

