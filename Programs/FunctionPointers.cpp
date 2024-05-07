
typedef int(*CMainPtr)(int count, char* args);

typedef int(__stdcall *CFunc1Ptr)(int arg1, int arg2);

int __stdcall func1(int arg1, int arg2)
{
	return 0;
}

/*
	Pointer to Member Function example
*/
struct CMemberFunctionTest
{
	int __stdcall Proc(int arg1, int arg2)
	{
		return 0;
	}
};

typedef int (__stdcall CMemberFunctionTest::* CMemberFunctionTestProcPtr)(int arg1, int arg2);

int FunctionPointers(int count, char* args)
{
	CMainPtr Ptr = FunctionPointers;

	CFunc1Ptr Ptr1 = func1;
	int Val = Ptr1(10, 11);

	CMemberFunctionTest Test1;
	CMemberFunctionTestProcPtr MemberFunctionTestProcPtr = &CMemberFunctionTest::Proc;
	Val = ((&Test1)->*MemberFunctionTestProcPtr)(10, 11);
	Val = (Test1.*MemberFunctionTestProcPtr)(10, 11);

	return 10;
}

