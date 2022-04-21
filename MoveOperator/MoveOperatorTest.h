
class CMoveOperatorTester
{

	class CTestObject
	{
		int *Q = 0;
	public:

		/*
		CTestObject(const CTestObject& CTestObject)
		{
			printf("Copy assignment constructor\r\n");
		}

		CTestObject& operator=(const CTestObject& CTestObject)
		{
			printf("Copy assignment operator\r\n");
		}
		*/

		CTestObject(const CTestObject& CTestObject) = delete;

		CTestObject& operator=(const CTestObject& CTestObject) = delete;

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
		
		CTestObject(CTestObject&& Other)
		{
			printf("Move assignment constructor\r\n");
			Q = Other.Q;
			Other.Q = 0;
		}

		CTestObject& operator=(CTestObject&& Other)
		{
			printf("Move assignment operator\r\n");
			Q = Other.Q;
			Other.Q = 0;
			return *this;
		}
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

	void Test()
	{
		printf("Move operator test has been started\r\n");

		CTestObject Q = f();

		printf("Function has been called\r\n");
		
		printf("Call the second function\r\n");

		CTestObject Q1 = f(CTestObject(new int()));

		printf("Move operator test has been completed\r\n");
	}
};