This application takes a string representing an equation and can then solve it for you with values you provide
to the variables.

In Version 2 (in progress), this will also rearrange the equation to solve for any variable in the provided equation.



========================================================================
    CONSOLE APPLICATION : Equations Project Overview
========================================================================

Equations.cpp
    This is the main application source file.

Equation
    Class holding information about the equation

EquationsSolver
    Handles parsing the strings into evaluable portions to provide a final answer calculated from provided values.
    
EquationParser
    Parses the equation string into portions that can later be used to assign values or solve the equation with
    
EquationRearranger
    Not yet implemented. Will rearrange to isolate a specified variable

EquationSharedData
    Information shared among the above 4 classes

StringExtensions
    Helper functions for specific string manipulations
/////////////////////////////////////////////////////////////////////////////
