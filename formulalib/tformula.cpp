#include "tformula.h"

size_t TFormula::Priority(const char& c) const
{
	if (c == '^')
		return 4;
	if (c == '*' || c == '/' || c == '^')
		return 3;
	if (c == '+' || c == '-')
		return 2;
	if (c == '(')
		return 1;
	return 0;
}

size_t TFormula::PriorityWithUnaryMinus(const char& c) const
{
	if (c == '~')
		return 5;
	return Priority(c);
}

bool TFormula::ReservedNamesFuntions(const std::string& str) const
{
	return (str == "sin" || str == "cos" || str == "tan" || str == "cot" || str == "log");
}

bool TFormula::ReservedOperations(const char& c) const
{
	return Priority(c) > 1;
}

bool TFormula::ReservedConstants(const std::string& str, double& res) const
{
	if (str == "pi")
	{
		res = M_PI;
		return true;
	}
	if (str == "e")
	{
		res = M_E;
		return true;
	}
	return false;
}

void TFormula::HandleTempNumberOrVarOrFunction(const size_t& sz, TStack<std::string>& errs, std::string& tmp, const size_t& i, const bool& FirstThrow) const
{
	size_t tmp_sz = tmp.size();
	if (tmp[0] == '.')
	{
		errs.Push(std::to_string(i - 1) + ": " + "Var or number can't start with '.' yet.");
		if (FirstThrow)
			throw errs.Top();
		tmp = "";
	}
	else if (tmp_sz != 0)
	{
		if (isdigit(tmp[0]))
		{
			bool found_dot = false;
			for (size_t j = 1; j < tmp_sz; j++)
			{
				if (tmp[j] == '.' && found_dot)
				{
					errs.Push(std::to_string(i - 1 - tmp_sz + j) + ": " + "Too many dots in one number.");
					if (FirstThrow)
						throw errs.Top();
					break;
				}
				else if (tmp[j] == '.' && !found_dot)
					found_dot = true;
				if (tmp[j] != '.' && !isdigit(tmp[j]))
				{
					errs.Push(std::to_string(i - 1 - tmp_sz + j) + ": " + "Var name can't start with a number.");
					if (FirstThrow)
						throw errs.Top();
					break;
				}
			}
			if (tmp[tmp_sz - 1] == '.')
			{
				errs.Push(std::to_string(i - 1) + ": " + "Autocorrection for a number that ends with '.' is not supported yet.");
				if (FirstThrow)
					throw errs.Top();
			}
		}
		else
		{
			bool is_reserved_fun_name = ReservedNamesFuntions(tmp);
			if (is_reserved_fun_name && (i == sz || fm[i] != '('))
			{
				errs.Push(std::to_string(i - 1) + ": " + "Illegal var name " + tmp + ". It's a reserved function name.");
				if (FirstThrow)
					throw errs.Top();
			}
			if (!is_reserved_fun_name && i != sz && fm[i] == '(')
			{
				errs.Push(std::to_string(i - 1) + ": " + "Var name " + tmp + " is not a function.");
				if (FirstThrow)
					throw errs.Top();
			}
		}
		tmp = "";
	}
}

bool TFormula::GetDefinedValues(const std::string& str, double& res) const
{
	for (size_t i = 0; i < def_vals_sz; i++)
		if (vars[i] == str)
		{
			res = vals[i];
			return true;
		}
	return false;
}

TFormula::TFormula(const std::string& form)
{
	SetFormula(form);
	//postfix = "";
	//result = 0.0;
	vars = nullptr;
	vals = nullptr;
	def_vals_sz = 0;
}

TFormula::TFormula(const TFormula& f)
{
	fm = f.fm;
	postfix = f.postfix;
	result = f.result;
	calculated = f.calculated;
	postfix_calculated = f.postfix_calculated;
	formula_checked = f.formula_checked;
	if (f.vars != nullptr && f.vals != nullptr && f.def_vals_sz > 0)
	{
		def_vals_sz = f.def_vals_sz;
		vars = new std::string[def_vals_sz];
		vals = new double[def_vals_sz];
		for (size_t i = 0; i < def_vals_sz; i++)
		{
			vars[i] = f.vars[i];
			vals[i] = f.vals[i];
		}
	}
	else
	{
		vars = nullptr;
		vals = nullptr;
		def_vals_sz = 0;
	}
}

TFormula::TFormula(TFormula&& f) noexcept
{
	this->operator=(f);
}

TFormula& TFormula::operator=(const TFormula& f)
{
	if (vars == f.vars || vals == f.vals)
		return *this;
	fm = f.fm;
	postfix = f.postfix;
	result = f.result;
	calculated = f.calculated;
	postfix_calculated = f.postfix_calculated;
	formula_checked = f.formula_checked;
	if (f.vars != nullptr && f.vals != nullptr && f.def_vals_sz > 0)
	{
		def_vals_sz = f.def_vals_sz;
		vars = new std::string[def_vals_sz];
		vals = new double[def_vals_sz];
		for (size_t i = 0; i < def_vals_sz; i++)
		{
			vars[i] = f.vars[i];
			vals[i] = f.vals[i];
		}
	}
	else
	{
		vars = nullptr;
		vals = nullptr;
		def_vals_sz = 0;
	}
	return *this;
}

TFormula& TFormula::operator=(TFormula&& f) noexcept
{
	vars = nullptr;
	vals = nullptr;
	def_vals_sz = 0;
	std::swap(fm, f.fm);
	std::swap(postfix, f.postfix);
	std::swap(vars, f.vars);
	std::swap(vals, f.vals);
	std::swap(def_vals_sz, f.def_vals_sz);
	std::swap(result, f.result);
	std::swap(calculated, f.calculated);
	std::swap(postfix_calculated, f.postfix_calculated);
	std::swap(formula_checked, f.formula_checked);
	return *this;
}

TFormula::~TFormula()
{
	delete[] vars;
	delete[] vals;
	def_vals_sz = 0;
	calculated = false;
	postfix_calculated = false;
	formula_checked = false;
}

void TFormula::SetFormula(const std::string& form)
{
	size_t sz = form.size();
	calculated = false;
	postfix_calculated = false;
	formula_checked = false;
	fm = "";
	for (size_t i = 0; i < sz; i++)
		if (form[i] != ' ')
			fm += form[i];
}

std::string TFormula::GetFormula(void) const
{
	return fm;
}

TStack<std::string> TFormula::CheckFormula(bool FirstThrow)
{
	size_t sz = fm.size();
	TStack<std::string> errs(sz);
	if (formula_checked)
		return errs;
	if (fm == "")
	{
		errs.Push("Formula is empty.");
		if (FirstThrow)
			throw errs.Top();
		return errs;
	}

	char typo = 0;
	int brackets = 0;
	std::string tmp = "";

	// First symbol check
	if (!isdigit(fm[0]) && !isalpha(fm[0]) && fm[0] != '.' && fm[0] != '_' && fm[0] != '(' && fm[0] != '-')
	{
		errs.Push("0: You can't start formula with " + fm[0] + '.');
		if (FirstThrow)
			throw errs.Top();
	}

	for (size_t i = 0; i < sz; i++)
	{
		size_t tmp_sz = tmp.size();
		if (!isdigit(fm[i]) && !isalpha(fm[i]) && fm[i] != '.' && fm[i] != '_')
		{
			if (fm[i] != '(' && fm[i] != ')')
			{
				if (typo != 0 && (typo != '(' || fm[i] != '-'))
				{
					errs.Push(std::to_string(i) + ": " + "Extra operation " + fm[i] + '.');
					if (FirstThrow)
						throw errs.Top();
				}
				typo = fm[i];
				if (!ReservedOperations(fm[i]) && fm[i] != '.' && fm[i] != '_')
				{
					errs.Push(std::to_string(i) + ": " + "Illegal character " + fm[i] + '.');
					if (FirstThrow)
						throw errs.Top();
				}
			}
			else
			{
				if (fm[i] == '(')
				{
					typo = '(';
					if (brackets >= 0)
						brackets++;
				}
				else if (fm[i] == ')')
				{
					if (i > 0 && fm[i - 1] == '(')
					{
						errs.Push(std::to_string(i) + ": " + "You can't have '()' in formula.");
						if (FirstThrow)
							throw errs.Top();
					}
					typo = 0;
					if (brackets >= 0)
						brackets--;
				}
				if (brackets < 0)
				{
					errs.Push(std::to_string(i) + ": " + "Extra ')'.");
					if (FirstThrow)
						throw errs.Top();
				}
			}
			HandleTempNumberOrVarOrFunction(sz, errs, tmp, i, FirstThrow);
		}
		else
		{
			typo = 0;
			tmp += fm[i];
		}
	}
	if (brackets > 0)
	{
		errs.Push("Missing ')'.");
		if (FirstThrow)
			throw errs.Top();
	}
	HandleTempNumberOrVarOrFunction(sz, errs, tmp, sz, FirstThrow);

	formula_checked = errs.IsEmpty();
	return errs;
}

void TFormula::CreatePostfixForm(void)
{
	if (postfix_calculated)
		return;
	if (!formula_checked)
		CheckFormula(true);
	postfix = "";
	size_t sz = fm.size();
	// ѕроблема унарного минуса.
	std::string _fm = fm;
	if (_fm[0] == '-')
		_fm[0] = '~';
	for (size_t i = 1; i < sz; i++)
		if (_fm[i] == '-' && _fm[i - 1] == '(')
			_fm[i] = '~';
	// ¬ постфиксной формуле всЄ должно раздел€тьс€ пробелами!!!
	TStack<std::string> opst(sz);
	bool handling_e = false;
	std::string tmp = "";
	std::string StackItem;
	bool is_operation;
	bool handling_function = false;

	for (size_t i = 0; i < sz; i++)
	{
		is_operation = ReservedOperations(_fm[i]) || _fm[i] == '~';
		if (!is_operation && _fm[i] != '(' && _fm[i] != ')')
		{
			tmp += _fm[i];
			handling_e = true;
		}
		else
		{
			if (handling_e)
			{
				if (ReservedNamesFuntions(tmp))
				{
					handling_function = true;
					i--;
				}
				else
				{
					handling_function = false;
					postfix += tmp + " ";
					tmp = "";
				}
			}
			handling_e = false;
			if (_fm[i] == '(')
			{
				opst.Push("(");
			}
			else if (_fm[i] == ')')
			{
				while ((tmp = opst.TopPop())[0] != '(')
				{
					handling_e = !(ReservedOperations(tmp[0]) || tmp[0] == '~' || ReservedNamesFuntions(tmp));
					postfix += tmp;
					if (!handling_e)
						postfix += ' ';
				}
			}
			else
			{
				size_t prio, prio_stacki;
				if (handling_function)
					prio = 5;
				else
					prio = PriorityWithUnaryMinus(_fm[i]);
				while (!opst.IsEmpty())
				{
					StackItem = opst.TopPop();
					handling_e = !(ReservedOperations(StackItem[0]) || StackItem[0] == '~' || ReservedNamesFuntions(StackItem));
					if (ReservedNamesFuntions(StackItem))
						prio_stacki = 5;
					else
						prio_stacki = PriorityWithUnaryMinus(StackItem[0]);
					if (prio <= prio_stacki)
					{
						postfix += StackItem;
						if (!handling_e)
							postfix += ' ';
					}
					else
					{
						opst.Push(StackItem);
						break;
					}
				}
				handling_e = !(is_operation || handling_function);
				if (handling_function)
					opst.Push(tmp);
				else
					opst.Push(std::string(1, _fm[i]));
				handling_function = false;
				tmp = "";
			}
		}
	}
	if (handling_e)
		postfix += tmp;
	if (!opst.IsEmpty() && postfix[postfix.size() - 1] != ' ')
		postfix += ' ';

	while (!opst.IsEmpty())
	{
		tmp = opst.TopPop();
		handling_e = !(ReservedOperations(tmp[0]) || tmp[0] == '~' || ReservedNamesFuntions(tmp));
		postfix += tmp;
		if (!handling_e && !opst.IsEmpty())
			postfix += ' ';
	}
	postfix_calculated = true;
}

std::string TFormula::GetPostfixForm(void)
{
	if (!postfix_calculated)
		CreatePostfixForm();
	return postfix;
}

double TFormula::Calculate(void)
{
	if (calculated)
		return result;
	if (!postfix_calculated)
		CreatePostfixForm();
	size_t sz = postfix.size();
	TStack<double> opst(sz);
	std::string tmp = "";
	for (size_t i = 0; i < sz + 1; i++)
	{
		if (postfix[i] == ' ' || i == sz)
		{
			if (Priority(tmp[0]) > 0)
			{
				double b = opst.TopPop();
				double a = opst.TopPop();
				switch (tmp[0])
				{
				case '+':
					opst.Push(a + b);
					break;
				case '-':
					opst.Push(a - b);
					break;
				case '*':
					opst.Push(a * b);
					break;
				case '/':
					if (std::fpclassify(b) == FP_ZERO)
						throw std::runtime_error("Division by zero.");
					opst.Push(a / b);
					break;
				case '^':
					opst.Push(std::pow(a, b));
					break;
				}
			}
			else if (tmp[0] == '~')
			{
				opst.Push(-opst.TopPop());
			}
			else if (ReservedNamesFuntions(tmp))
			{
				double a = opst.TopPop();
				if (tmp == "sin")
					opst.Push(std::sin(a));
				else if (tmp == "cos")
					opst.Push(std::cos(a));
				else if (tmp == "tan")
				{
					if (std::fpclassify(std::cos(a)) == FP_ZERO) // Ёто бесполезно, косинус никогда не будет равен нулю.
						throw std::runtime_error("Division by zero.");
					opst.Push(std::tan(a));
				}
				else if (tmp == "cot")
				{
					if (std::fpclassify(std::sin(a)) == FP_ZERO) // Ёто бесполезно, синус никогда не будет равен нулю.
						throw std::runtime_error("Division by zero.");
					opst.Push(1.0 / (std::tan(a)));
				}
				else if (tmp == "log")
					opst.Push(std::log(a));
				else
					throw "I am sorry, it seems that I forgot about some functions. Ping me pls.";
			}
			else
			{
				double a;
				try
				{
					a = std::stod(tmp);
					opst.Push(a);
				}
				catch (std::invalid_argument)
				{
					if (GetDefinedValues(tmp, a))
					{
						opst.Push(a);
					}
					else if (ReservedConstants(tmp, a))
					{
						opst.Push(a);
					}
					else
					{
						throw tmp + " is not defined.";
					}
				}
				
			}
			tmp = "";
		}
		else
		{
			tmp += postfix[i];
		}
	}
	result = opst.Top();
	calculated = true;
	return result;
}

void TFormula::SetDefinedValues(const std::string* _vars, const double* _vals, const size_t sz)
{
	if (sz <= 0)
	{
		throw "Size cannot be zero";
	}
	if (sz != def_vals_sz)
	{
		if (vars != nullptr || vals != nullptr)
		{
			delete[] vars;
			delete[] vals;
		}
		vars = new std::string[sz];
		vals = new double[sz];
	}
	def_vals_sz = sz;
	for (size_t i = 0; i < sz; i++)
	{
		vars[i] = _vars[i];
		vals[i] = _vals[i];
	}
}
