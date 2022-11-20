// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
//
//
//

#ifndef __TStack_H__
#define __TStack_H__

#include <iostream>

const size_t MAX_STACK_SIZE = 100000000;

template <typename T>
class TStack
{
protected:
	T* pMem = nullptr;
	size_t MemSize;
	size_t Hi;
	const static size_t unzero = ~0;
public:
	TStack(size_t sz = 10);
	TStack(const TStack& s);
	TStack(TStack&& s) noexcept;
	TStack& operator=(const TStack& s);
	TStack& operator=(TStack&& s) noexcept;
	~TStack();
	
	bool IsEmpty(void) const noexcept;
	bool IsFull(void) const noexcept;

	void Push(const T& e);
	T Top(void) const;
	void Pop(void);
	T TopPop(void);

	bool operator==(const TStack& s) const noexcept;
	bool operator!=(const TStack& s) const noexcept;
};

template<typename T>
inline TStack<T>::TStack(size_t sz) : MemSize(sz)
{
	if (MemSize == 0)
		throw std::out_of_range("Stack size should be greater than zero");
	if (MemSize > MAX_STACK_SIZE)
		throw std::out_of_range("Stack size should be less than MAX_STACK_SIZE");
	Hi = unzero;
	pMem = new T[MemSize](); // У типа T д.б. констуктор по умолчанию
}

template<typename T>
inline TStack<T>::TStack(const TStack& s)
{
	if (s.pMem == nullptr)
	{
		MemSize = 0;
		pMem = nullptr;
		Hi = unzero;
	}
	else
	{
		MemSize = s.MemSize;
		Hi = s.Hi;
		pMem = new T[MemSize];
		if (!(s.IsEmpty()))
			std::copy(s.pMem, s.pMem + Hi + 1, pMem);
	}
}

template<typename T>
inline TStack<T>::TStack(TStack&& s) noexcept
{
	pMem = nullptr;
	std::swap(pMem, s.pMem);
	std::swap(MemSize, s.MemSize);
	std::swap(Hi, s.Hi);
}

template<typename T>
inline TStack<T>& TStack<T>::operator=(const TStack& s)
{
	if (this == &s)
		return *this;
	if (MemSize != s.MemSize)
	{
		T* tmp = new T[s.MemSize];
		delete[] pMem;
		MemSize = s.MemSize;
		pMem = tmp;
	}
	Hi = s.Hi;
	if (!(s.IsEmpty()))
		std::copy(s.pMem, s.pMem + Hi + 1, pMem);
	return *this;
}

template<typename T>
inline TStack<T>& TStack<T>::operator=(TStack&& s) noexcept
{
	std::swap(pMem, s.pMem);
	std::swap(MemSize, s.MemSize);
	std::swap(Hi, s.Hi);
	s.pMem = nullptr;
	return *this;
}

template<typename T>
inline TStack<T>::~TStack()
{
	delete[] pMem;
	MemSize = 0;
	Hi = unzero;
}

template<typename T>
inline bool TStack<T>::IsEmpty(void) const noexcept
{
	return (Hi == unzero);
}

template<typename T>
inline bool TStack<T>::IsFull(void) const noexcept
{
	return ((Hi + 1) == MemSize);
}

template<typename T>
inline void TStack<T>::Push(const T& e)
{
	if (this->IsFull())
		throw std::out_of_range("Stack if full.");
	pMem[++Hi] = e;
}

template<typename T>
inline T TStack<T>::Top(void) const
{
	if (this->IsEmpty())
		throw std::out_of_range("Stack if empty.");
	return pMem[Hi];
}

template<typename T>
inline void TStack<T>::Pop(void)
{
	if (this->IsEmpty())
		throw std::out_of_range("Stack if empty.");
	Hi--;
}

template<typename T>
inline T TStack<T>::TopPop(void)
{
	if (this->IsEmpty())
		throw std::out_of_range("Stack if empty.");
	return pMem[Hi--];
}

template<typename T>
inline bool TStack<T>::operator==(const TStack& s) const noexcept
{
	if (this->IsEmpty() && s.IsEmpty())
		return true;
	if (Hi != s.Hi) // MemSize не важен
		return false;
	size_t tmp = Hi + 1;
	for (size_t i = 0; i < tmp; i++)
		if (pMem[i] != s.pMem[i])
			return false;
	return true;
}

template<typename T>
inline bool TStack<T>::operator!=(const TStack& s) const noexcept
{
	return !(this->operator==(s));
}
#endif
