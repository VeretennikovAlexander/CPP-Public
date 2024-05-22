
#include <vector>
#include <string_view>
#include <iterator>
#include <concepts>

/*
Standard concepts:

same_as, convertible_to, derived_from,  
integral, floating_point, is_arithmetic,
is_copy_constructible, is_move_constructible, is_constructible, is_assignable
equality_comparable, totally_ordered,
movable, copyable, invocable, predicate,
forward_iterator, random_access_iterator, incrementable, 
indirectly_copyable, indirectly_swappable
...
*/

class CMyClass0
{

}; 

/*
Requires expression:

	requires (parameter-list) { requirements; }

	(parameter-list) is optional.

	requirements; is a sequence of requirements;

	Requires expression can be simple, compound, type, or nested.
*/

/**
	Simple (the methods must be const)
*/
template <typename T>
concept CMyConcept0 = requires(const T t)
{
	t.print();
	t.size();
};

/**
	Compound (the methods must be const).

	Note. convertible_to is a standard concept.
*/
template <typename T>
concept CMyConcept1 = requires(const T t)
{
	{ t.size() } -> convertible_to<size_t>;
};

/**
	Simple (the methods can be non-const)
*/
template <typename T>
concept CMyConcept2 = requires(T t)
{
	t++;
	t--;
	t.print_no_const();
	
	/*
		Nested
	*/
	requires sizeof(t) == 4;
};

/*
	Simple
*/
template <typename T>
concept CMyConcept3 = (sizeof(T) == 4);

/*
	Using existing concepts.
*/
template <typename T>
concept CMyConcept4 = derived_from<T, CMyClass0 >;

/*
	Simple, check a field.
*/
template <typename T>
concept CMyConcept5 = requires(T t)
{
	T::Val;
};

/*
	Type requirement (Check that a some type is valid).
*/
template <typename T>
concept CMyConcept6 = requires {typename T::value_type;  };

/*
	We can use && or || to combine concepts.
*/
template <typename T>
concept CMyConcept = requires(const T t)
{
	t.print();	
	{ t.size() } -> convertible_to<size_t>;
}
&& 
requires(T t)
{
	t++;
	t--;
	t.print_no_const();
} && (sizeof(T) == 4) && derived_from<T, CMyClass0 > 
&& 
requires {T::Val; }
&&
requires {typename T::value_type;  }
&& CMyConcept1<T>;

class CMyClass1: public CMyClass0
{
public:

	typedef int value_type;

	value_type Val = 0;

	CMyClass1 operator++(int)
	{
		CMyClass1 Res = *this;
		Res.Val++;
		return Res;
	}

	CMyClass1 operator--(int)
	{
		CMyClass1 Res = *this;
		Res.Val--;
		return Res;
	}

	void print_no_const()
	{

	}

	void print() const
	{

	}

	size_t size() const
	{
		return 0;
	}
};

template <class T> requires CMyConcept0<T>
class CMyClass3: public T
{
};

/*
	Template with concepts.
*/
template <class T> 
requires CMyConcept<T>&&
CMyConcept0<T>&&
CMyConcept1<T>&&
CMyConcept2<T>&&
CMyConcept3<T>&&
CMyConcept4<T>&&
CMyConcept5<T>&&
CMyConcept6<T>
class CMyClass2 : public T
{

};

/*
	Several arguments
*/
template <typename T>
concept Comparable = requires(const T a, const T b) 
{
	{ a == b } -> same_as<bool>;
	{ a > b } -> same_as<bool>;
	{ a < b } -> same_as<bool>;
};

template <class T> requires Comparable<T>
class CComparableTest
{

};

class CConceptTest
{
public:

	
	template <class T> requires incrementable<T> && (sizeof(T) == 4)
	void process(const T& t) {
		printf("Incrementable %d\r\n", (int)(int32_t)t);
	}

	template <class T> requires CMyConcept<T>
	void process(T& t)
	{
		printf("CMyConcept: %d\r\n", t.size());
	}

	template <typename T> requires derived_from<T, string>
	void process(const T& t)
	{
		printf("Str %s\r\n", t.c_str());
	}


	template <typename T> requires requires(T t) { t++; t--; }
	void process1(const T& t)
	{

	}

	template <typename T> requires (sizeof(T) == 4)
		void process2(const T& t)
	{

	}

	template <typename T> requires is_arithmetic_v<T>
	void process3(const T& t)
	{

	}

	template <incrementable T>
	void process_incrementable(const T& t) {

	}

	void Test1()
	{
		string Str;
		int x;
		float f;
		process(x);
		process(Str);
		process1(x);
		process2(f);
		process3(x);
		process_incrementable(x);

		CMyClass1 Obj1;
		process(Obj1);

		CMyClass2<CMyClass1> Obj2;
		CMyClass3<CMyClass1> Obj3;

		CComparableTest<int> Obj4;
	}
};
