// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
//
//
// Тестирование стека

#include <iostream>
#include "tstack.h"

using namespace std;

int main(int argc, char *argv[])
{
	TStack<int> test(10);
	for (size_t i = 0; i < 5; i++)
		test.Push(i);
	cout << "Stack's top = " << test.Top() << endl;
	test.Pop();
	cout << "Stack's top now = " << test.Top() << endl;
	test.Push(123);
	cout << "Stack's top now = " << test.Top() << endl;
	return 0;
}