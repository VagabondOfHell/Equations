#pragma once
#include <string>

class Variable
{
	double value;
	bool isUnknown;
public:

	std::string Name;
	std::string Alias;

	Variable(void)
		:value(0.0), Alias(""), Name(""), isUnknown(true)
	{
	}

	Variable(const std::string& alias, const std::string& name = "", bool applyNegative = false)
		:value(0.0), Alias(alias), Name(name), isUnknown(true)
	{

	}

	///<summary>Creates a Variable with values set. Assumes that it is not an unknown value</summary>
	///<param name="_val">The value of the variable</param>
	///<param name="alias">The letter id of the variable (i.e. y or x). Can be multi-character long</param>
	///<param name="name">The name that the variable represents</param>
	///<param name="applyNegative">True if a negative value should be applied</param>
	Variable(double _val, const std::string& alias, const std::string name = "")
		:value(_val), isUnknown(false), Alias(alias), Name(name)
	{

	}

	virtual ~Variable(void)
	{
	}

	inline bool IsKnown()const{return !isUnknown;}
	inline bool IsUnknown()const {return isUnknown;}

	inline void SetUnknown(){isUnknown = true; value = 0.0;}
	
	inline void SetValue(double val)
	{
		value = val;
		isUnknown = false;
	}

	inline double GetValue()
	{
		if(isUnknown)
			return 0.0;

		return value;
	}

};

