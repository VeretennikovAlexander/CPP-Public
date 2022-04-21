#pragma once

class CAutoPtrTester
{
	class CTest
	{
		size_t Id;
	public:

		CTest(size_t Id1)
		{
			Id = Id1;
			printf("Test %d has been created\n", (int)Id);
		}

		~CTest()
		{
			printf("Test %d has been destroyed\n", (int)Id);
		}
	};

	vector<auto_ptr<CTest> > Data;

public:

	void Test()
	{
		auto_ptr<CTest> P(new CTest(1));
		auto_ptr<CTest> P1 = P;
		printf("Auto ptrs have been created\n");
	}

};