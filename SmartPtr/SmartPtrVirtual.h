

/*
  А можно ли реализовать умный указатель через виртуальные функции?

  Проблемы
  1) Увеличение размера объекта умного указателя из-за ссылки на таблицу виртуальных функций.
  2) Если определяем операторы в базовом классе, они не наследуются нормально.
  Мешает декомпозиции кода.
*/

template <class T>
class CAbstractSmartPtrVirtual
{
protected:
	
	T* Ptr;

public:

	CAbstractSmartPtrVirtual(T* APtr) : Ptr(APtr)
	{

	}

	T* operator->()
	{
		return Ptr;
	}

	virtual ~CAbstractSmartPtrVirtual()
	{
		
	}
	
	CAbstractSmartPtrVirtual& operator=(const CAbstractSmartPtrVirtual& Other)
	{		
		Detach();
		Attach(Other);

		return *this;
	}

protected:

	virtual void Attach(const CAbstractSmartPtrVirtual& Other) = 0;

	virtual void Detach() = 0;

};

template <class T>
class CSmartPtrVirtual final: public CAbstractSmartPtrVirtual<T>
{
	size_t* Counter;

protected:

	void Attach(const CAbstractSmartPtrVirtual& Other1) override
	{
		CSmartPtrVirtual& Other = (CSmartPtrVirtual&)Other1;
		Counter = Other.Counter;
		(*Counter)++;
		Ptr = Other.Ptr;
	}
	
	void Detach() override
	{
		if (Ptr)
		{
			(*Counter)--;
			if ((*Counter) == 0)
			{
				delete Ptr;
				delete Counter;
			}
			Ptr = 0;
			Counter = 0;
		}
	}
public:

	explicit CSmartPtrVirtual(T* APtr) : CAbstractSmartPtrVirtual(APtr)
	{
		Counter = new size_t();
		*Counter = 1;
	}

	CSmartPtrVirtual(const CSmartPtrVirtual& Other)
	{
		Attach(Other);
	}
	
	virtual ~CSmartPtrVirtual()
	{
		Detach();
	}

};

class CSmartPtrVirtualTester
{
public:

	void Test()
	{
		struct CTest
		{
			int V;
		};
		CSmartPtrVirtual<CTest> P(new CTest());
		CSmartPtrVirtual<CTest> P1(new CTest());
		P->V = 0;
		P1 = P;
		printf("Smart Ptr Virtual Size: %d\r\n", (int)sizeof(P));
	}
};