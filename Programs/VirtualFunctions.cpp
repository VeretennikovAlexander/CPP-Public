
#include <stdio.h>
#include <memory>

using namespace std;

class CNonVirtual
{
public:
	
	int Data = 0;

};

/*
	__declspec(novtable) is optional in this example
*/
class __declspec(novtable) CBase
{
public:

	int Data = 0;

	CBase()
	{
		printf("CBase::CBase\n");
		f1("CBase::CBase");
	}

	void Test()
	{
		f1("CBase::Test");
	}

	virtual ~CBase()
	{
		printf("CBase::~CBase\n");
		f1("CBase::~CBase");
	}

	virtual void f1(char* Source)
	{
		printf("CBase::f1 (%s)\n", Source);
	}
};

class CClass1 : public CBase
{
public:

	CClass1()
	{
		printf("CClass1::CClass1\n");
	}

	~CClass1()
	{
		printf("CClass1::~CClass1\n");
	}

	virtual void f1(char* Source) override
	{
		printf("CClass1::f1 (%s)\n", Source);
	}
};

namespace CNonVirtualDestructors
{
	class CBase
	{
	public:

		~CBase()
		{
			printf("CNonVirtualDestructors::CBase::~CBase\n");
		}
	};

	class CClass1 : public CNonVirtualDestructors::CBase
	{
	public:

		~CClass1()
		{
			printf("CNonVirtualDestructors::CClass1::~CClass1\n");
		}
	};

}

/*
Output:

sizeof(CNonVirtual) 4, offsetof(CNonVirtual, Data) 0
sizeof(CBase) 8, offsetof(CBase, Data) 4
sizeof(CClass1) 8, offsetof(CClass1, Data) 4
CBase::CBase
CBase::f1 (CBase::CBase)
CClass1::CClass1
CClass1::f1 (CBase::Test)
CClass1::~CClass1
CBase::~CBase
CBase::f1 (CBase::~CBase)
CNonVirtualDestructors::CBase::~CBase

Notes:
	No call: CNonVirtualDestructors::CClass1::~CClass1.
*/


void VirtualFunctionsTest()
{
	{
		printf("sizeof(CNonVirtual) %d, offsetof(CNonVirtual, Data) %d\n",
			sizeof(CNonVirtual), offsetof(CNonVirtual, Data));
		printf("sizeof(CBase) %d, offsetof(CBase, Data) %d\n",
			sizeof(CBase), offsetof(CBase, Data));
		printf("sizeof(CClass1) %d, offsetof(CClass1, Data) %d\n",
			sizeof(CClass1), offsetof(CClass1, Data));

		CBase* Ptr = new CClass1();

		Ptr->Test();

		delete Ptr;
	}

	{
		CNonVirtualDestructors::CBase* T = new CNonVirtualDestructors::CClass1();
		delete T;
	}
}