// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
//
//
//

#ifndef __TFormula_H__
#define __TFormula_H__

#define _USE_MATH_DEFINES
#include "tstack.h"
#include <iostream>
#include <string>
#include <math.h>

const size_t FORMULA_MAX_LENGTH = 255;

class TFormula
{
protected:
	std::string fm;
	std::string postfix;
	std::string* vars;
	double* vals;
	size_t def_vals_sz;
	double result;
	bool calculated;
	bool postfix_calculated;
	bool formula_checked;
	size_t Priority(const char& c) const;
	size_t PriorityWithUnaryMinus(const char& c) const;
	bool ReservedNamesFuntions(const std::string& str) const;
	bool ReservedOperations(const char& c) const;
	bool ReservedConstants(const std::string& str, double& res) const;
	void HandleTempNumberOrVarOrFunction(const size_t& sz, TStack<std::string>& errs, std::string& tmp, const size_t& i, const bool& FirstThrow) const;
	bool GetDefinedValues(const std::string& str, double& res) const;
public:
	TFormula(const std::string& form = "");
	TFormula(const TFormula& f);
	TFormula(TFormula&& f) noexcept;
	TFormula& operator=(const TFormula& f);
	TFormula& operator=(TFormula&& f) noexcept;
	~TFormula();

	void SetFormula(const std::string& form);
	std::string GetFormula(void) const;
	TStack<std::string> CheckFormula(bool FirstThrow = false);
	void CreatePostfixForm(void);
	std::string GetPostfixForm(void);
	double Calculate(void);
	void SetDefinedValues(const std::string* _vars, const double* _vals, const size_t sz);
};

#endif