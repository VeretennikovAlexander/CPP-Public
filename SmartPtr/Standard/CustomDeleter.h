
class CCustomDeleterTester1
{
public:

	class CTest
	{
	public:

		CTest()
		{
			printf("Test has been created\n");
		}

		~CTest()
		{
			printf("Test has been deleted\n");
		}
	};

	void Test()
	{
		printf("Custom deleter\n");

		void* Buffer = malloc(sizeof(CTest));
		CTest* Test = new (Buffer) CTest();
		shared_ptr<CTest> Ptr(Test, [](CTest* Test) {
			Test->~CTest();
			free(Test);
			});
		

	}
};