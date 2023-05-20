#pragma once


struct C32Bit
{
	static void DoSomething()
	{
		printf("Do something x86\r\n");
	}
};

struct C64Bit
{
	static void DoSomething()
	{
		printf("Do something x64\r\n");
	}
};

class CAutoBit : public conditional<sizeof(void*) == 8, C64Bit, C32Bit>::type
{

};

template <bool Value, class T, class U>
struct CSelect
{
	typedef T Result;
};
template <class T, class U>
struct CSelect<false, T, U>
{
	typedef U Result;
};

class CAutoBit2 : public CSelect<sizeof(void*) == 8, C64Bit, C32Bit>::Result
{

};

void ConditionalTest()
{
	CAutoBit::DoSomething();
	CAutoBit2::DoSomething();
}
