// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
//
//
// Тестирование арифметических выражений

#include <iostream>
#include "tformula.h"

int main(int argc, char* argv[])
{
	TFormula test;
	test.SetFormula("a+b*cos(c)");
	test.CreatePostfixForm();
	std::cout << "Postfix form for " << test.GetFormula() << " is " << test.GetPostfixForm() << std::endl;
	std::string vars[] = { "a", "b", "c" };
	double vals[] = { 1.0, 2.2, 2.4 };
	std::cout << "Let ";
	for (size_t i = 0; i < 2; i++)
	{
		std::cout << vars[i] << " = " << vals[i] << ", ";
	}
	std::cout << vars[2] << " = " << vals[2] << ".\n";
	test.SetDefinedValues(vars, vals, 3);
	std::cout << "Then " << test.GetFormula() << " is equal to " << test.Calculate() << std::endl;
	return 0;
}