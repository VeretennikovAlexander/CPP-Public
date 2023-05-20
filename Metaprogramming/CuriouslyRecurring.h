#pragma once



template <class T>
class CBase1
{
	T* Owner()
	{
		return static_cast<T*>(this);
	}

public:

	void Test()
	{
		Owner()->Lock();
		//

		Owner()->UnLock();
	}
};

class CDerived1: public CBase1<CDerived1>
{
public:

	void Lock()
	{
		printf("Lock\r\n");
	}

	void UnLock()
	{
		printf("UnLock\r\n");
	}
};


class CSingleThreadedModel
{
public:

	void Lock()
	{
		printf("Lock\r\n");
	}

	void UnLock()
	{
		printf("UnLock\r\n");
	}
};
	
template <class CThreadedModel = CSingleThreadedModel>
class CDerived : public CBase1<CDerived<CThreadedModel>>,
	public CThreadedModel
{
public:

		
};


void CuriouslyReccurringTest()
{
	CDerived<> Test;
	Test.Test();

	CDerived1 Test1;
	Test1.Test();
}
