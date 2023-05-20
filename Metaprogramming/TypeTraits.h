#pragma once

#include <typeinfo>

template <class... Tn> class CTypeInfoPrint;

template <>
struct CTypeInfoPrint<>
{
	static void Print()
	{

	}
};

template <class T, class... Tn>
struct CTypeInfoPrint<T, Tn...>
{
	static void Print()
	{
		if constexpr (is_unsigned<T>::value)
		{
			printf("%s: min %llu, max %llu\n", typeid(T).name(), (uint64_t)(numeric_limits<T>::min)(), (uint64_t)(numeric_limits<T>::max)());
		}
		else
		{
			printf("%s: min %lld, max %lld\n", typeid(T).name(), (int64_t)(numeric_limits<T>::min)(), (int64_t)(numeric_limits<T>::max)());
		}
		CTypeInfoPrint<Tn...>::Print();
	}
};


void TypeTraitsTest()
{
	struct C1
	{

	};

	//Type traits
	printf("Is pointer %d\n", is_pointer<void*>::value);
	printf("Is pointer %d\n", is_pointer<int>::value);
	printf("Is unsigned %d\n", is_unsigned<unsigned int>::value);
	printf("Is unsigned %d\n", is_unsigned<int>::value);
	printf("Is integral %d\n", is_integral<int>::value);
	printf("Is integral %d\n", is_integral<C1>::value);

	printf("Is class %s, %d\n", typeid(int).name(), is_class<int>::value);
	printf("Is class %s, %d\n", typeid(C1).name(), is_class<C1>::value);
	printf("Max %s, %d\n", typeid(int).name(), (numeric_limits<int>::max)());
	printf("Max %s, %u\n", typeid(unsigned).name(), (numeric_limits<unsigned>::max)());

	CTypeInfoPrint<char, unsigned char, short, unsigned short, int, unsigned int, int64_t, uint64_t, intmax_t, uintmax_t>::Print();
}
