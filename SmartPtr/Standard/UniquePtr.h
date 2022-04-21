#pragma once

class CUniquePtrTester
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

	vector<unique_ptr<CTest> > Data;
	
public:

	void Test()
	{
		//unique_ptr<CTest> P1(new CTest(-1));

		for (size_t i = 0; i < 3; i++)
		{
			unique_ptr<CTest> P = make_unique<CTest>(i);
			Data.push_back(move(P));
		}
		
		printf("Unique ptrs have been created\n");

		{
			unique_ptr<CTest> P = move(Data[0]);
		}

		printf("An unique ptr has been extracted\n");
	}

};