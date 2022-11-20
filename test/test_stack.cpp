#include "tstack.h"

#include <gtest.h>

TEST(TStack, can_create_stack_with_positive_length)
{
	ASSERT_NO_THROW(TStack<int> s(5));
}

TEST(TStack, cant_create_too_large_stack)
{
	ASSERT_ANY_THROW(TStack<int> s(MAX_STACK_SIZE + 1));
}

TEST(TStack, throws_when_create_stack_with_negative_length)
{
	ASSERT_ANY_THROW(TStack<int> s(-5));
}

TEST(TStack, can_create_copied_stack)
{
	TStack<int> s1(10);
	ASSERT_NO_THROW(TStack<int> s2(s1));
}

TEST(TStack, copied_stack_is_equal_to_source_one)
{
	TStack<int> s1(10);
	s1.Push(2);
	s1.Push(5);
	s1.Push(11);
	TStack<int> s2(s1);
	EXPECT_EQ(true, s1 == s2);
}

TEST(TStack, copied_stack_has_its_own_memory)
{
	TStack<int> s1(10);
	for (size_t i = 0; i < 3; i++)
		s1.Push(i);
	TStack<int> s2(s1);
	s2.Pop();
	s2.Push(123);
	EXPECT_EQ(true, s1 != s2);
}

TEST(TStack, can_push_and_top_element)
{
	TStack<int> s(5);
	s.Push(5);
	s.Push(6);
	EXPECT_EQ(6, s.Top());
}

TEST(TStack, can_pop_element)
{
	TStack<int> s(5);
	s.Push(5);
	s.Push(6);
	s.Pop();
	EXPECT_EQ(5, s.Top());
}

TEST(TStack, can_top_pop_element)
{
	TStack<int> s(5);
	s.Push(5);
	s.Push(6);
	EXPECT_EQ(6, s.TopPop());
	EXPECT_EQ(5, s.TopPop());
}

TEST(TStack, knows_if_empty)
{
	TStack<int> s(5);
	EXPECT_EQ(true, s.IsEmpty());
	s.Push(11);
	EXPECT_EQ(false, s.IsEmpty());
	s.Pop();
	EXPECT_EQ(true, s.IsEmpty());
}

TEST(TStack, knows_if_full)
{
	TStack<int> s(5);
	EXPECT_EQ(false, s.IsFull());
	s.Push(0);
	EXPECT_EQ(false, s.IsFull());
	for (size_t i = 1; i < 5; i++)
		s.Push(i * 2);
	EXPECT_EQ(true, s.IsFull());
}

TEST(TStack, throws_when_push_when_full)
{
	TStack<int> s(5);
	for (size_t i = 0; i < 5; i++)
		s.Push(i * 2);
	ASSERT_ANY_THROW(s.Push(0));
}

TEST(TStack, throws_when_pop_when_empty)
{
	TStack<int> s(5);
	s.Push(0);
	s.Pop();
	ASSERT_ANY_THROW(s.Pop());
}

TEST(TStack, can_assign_stack_to_itself)
{
	TStack<int> s(5);
	ASSERT_NO_THROW(s = s);
}

TEST(TStack, can_assign_stack_of_equal_size)
{
	TStack<int> s1(5), s2(5);
	s1.Push(15);
	s1.Push(16);
	s2 = s1;
	s2.Pop();
	s2.Pop();
	s2.Push(16);
	s2.Push(15);
	EXPECT_NE(16, s2.TopPop());
	EXPECT_NE(15, s2.Top());
}

TEST(TStack, can_assign_stack_of_different_size)
{
	TStack<int> s1(2), s2(1);
	s1.Push(100);
	s1.Push(200);
	s2.Push(0);
	s2 = s1;
	EXPECT_EQ(200, s2.TopPop());
	EXPECT_EQ(100, s2.TopPop());
}

TEST(TStack, compare_equal_stacks_return_true)
{
	TStack<int> s1(5), s2(5);
	s1.Push(5);
	s1.Push(6);
	s1.Push(7);

	s2.Push(5);
	s2.Push(6);
	s2.Push(7);
	EXPECT_EQ(true, s1 == s2);
}

TEST(TStack, compare_stack_with_itself_return_true)
{
	TStack<int> s(5);
	s.Push(0);
	EXPECT_EQ(true, s == s);
}

TEST(TStack, stacks_with_different_size_are_not_equal)
{
	TStack<int> s1(5), s2(5);
	s1.Push(5);
	s1.Push(6);
	s2.Push(5);
	EXPECT_EQ(true, s1 != s2);
}

TEST(TStack, same_stacks_but_with_different_memory_size_are_equal)
{
	TStack<int> s1(5), s2(10);
	s1.Push(5);
	s1.Push(6);
	s2.Push(5);
	s2.Push(6);
	EXPECT_EQ(true, s1 == s2);
}