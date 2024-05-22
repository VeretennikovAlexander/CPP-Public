
#include <vector>
#include <type_traits>

#define MOVE_OPERATOR

class CMoveOperatorTester
{

	class CTestObject
	{
		int *Q = 0;
	public:

#ifndef MOVE_OPERATOR
		CTestObject(const CTestObject& Other)
		{
			if (Other.Q)
				Q = new int(*Other.Q);
			printf("Copy constructor %I64d\r\n", (uint64_t)Other.Q);
		}

		CTestObject& operator=(const CTestObject& Other)
		{
			if (Q)
			{
				delete Q;
				Q = 0;
			}
			if (Other.Q)
				Q = new int(*Other.Q);
			printf("Copy assignment operator %I64d\r\n", (uint64_t)Other.Q);
			return *this;
		}
#else

		CTestObject(const CTestObject& CTestObject) = delete;

		CTestObject& operator=(const CTestObject& CTestObject) = delete;
#endif

		CTestObject(int *R)
		{
			Q = R;
			printf("Test object has been created %I64d\r\n",(uint64_t)R);
		}

		~CTestObject()
		{
			printf("Test object has been destroyed, %I64d\r\n", (uint64_t)Q);
			if (!Q)
				delete Q;
		}

		/*	
			Move конструктор не генерится сам по себе, если
			определен конструктор или оператор копирования.
		*/
		/*
			Если комментируем, использует конструктор копирования.
		*/
		
#ifdef MOVE_OPERATOR
		CTestObject(CTestObject&& Other)
		{
			printf("Move constructor %I64d\r\n", (uint64_t)Other.Q);
			Q = Other.Q;
			Other.Q = 0;
		}

		CTestObject& operator=(CTestObject&& Other)
		{
			if (this == &Other)
				return *this;

			printf("Move assignment operator %I64d -> %I64d\r\n", (uint64_t)Q, (uint64_t)Other.Q);
			Q = Other.Q;
			Other.Q = 0;
			return *this;
		}

#endif
	};

public:

	CTestObject f()
	{
		CTestObject V(new int());
		return V;
	}

	CTestObject f(CTestObject x)
	{
		return x;
	}

	/*
	Example output:

	Move operator test has been started
	Vector has been created
	Test object has been created 3136296
	Move constructor 3136296
	Test object has been destroyed, 0
	Test object has been created 3136072
	Move constructor 3136072
	Move constructor 3136296
	Test object has been destroyed, 0
	Test object has been destroyed, 0
	Vector has been populated
	Test object has been created 3136232
	Test object has been created 3136216
	Test objects A and B have been created
	Move assignment operator 3136232 -> 3136216
	B has been moved to A
	Test object has been created 3136360
	Function has been called
	The second function call
	Test object has been created 3136248
	Move constructor 3136248
	Test object has been destroyed, 0
	Move operator test has been completed
	Test object has been destroyed, 3136248
	Test object has been destroyed, 3136360
	Test object has been destroyed, 0
	Test object has been destroyed, 3136216
	Test object has been destroyed, 3136296
	Test object has been destroyed, 3136072

	*/
	void Test()
	{
		printf("Move operator test has been started\r\n");

		printf("CTestObject is_move_constructable %d\r\n", (int)is_move_constructible<CTestObject>::value);

		vector< CTestObject> Vec;
		printf("Vector has been created\n");

		Vec.push_back(CTestObject(new int()));
		Vec.push_back(CTestObject(new int()));

		printf("Vector has been populated\n");

		CTestObject A(new int()), B(new int());

		printf("Test objects A and B have been created, move B->A\n");

		A = move(B);

		printf("B has been moved to A\n");

		CTestObject Q = f();

		printf("Function has been called\r\n");
		
		printf("The second function call\r\n");

		CTestObject Q1 = f(CTestObject(new int()));

		printf("Move operator test has been completed\r\n");
	}
};