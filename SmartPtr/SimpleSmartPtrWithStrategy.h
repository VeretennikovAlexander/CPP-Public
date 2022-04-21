/*

Пример для курса ФИЛП, УрФУ, 2017-2018. Веретенников А. Б.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


template <class T>
class CCounterStrategy
{
	size_t* Counter;

public:

	/**
		Вызывается при создании умного указателя на основании другого умного указателя.
		Параметр - значение обычного указателя для умного указателя - источника.
		Возвращает значение обычного указателя.

		Будем предполагать, что Ptr может быть 0.
	*/
	T* AddRef(const CCounterStrategy& Other, T* Ptr)
	{
		Counter = Other.Counter;
		size_t R = ++(*Counter);
		return Ptr;
	}

	/*
		Вызывается при создании умного указателя на основании обычного указателя.

		Будем предполагать, что Ptr может быть 0.
	*/
	void InitRef(T* Ptr)
	{
		Counter = new size_t();
		*Counter = 1;
	}

	/*
		Если возвращает 0, требуется освободить ресурс.

		Будем предполагать, что Ptr может быть 0.
	*/
	size_t Release(T* Ptr)
	{
		if (!Counter)
		{
			return 0;
		}

		size_t R = --(*Counter);

		if (R == 0)
		{
			delete Counter;
		}

		Counter = 0;
		return R;
	}
};

template <class T>
class CLinkedListStrategy
{
public:

	CLinkedListStrategy* Next;
	CLinkedListStrategy* Prev;

	T* AddRef(const CLinkedListStrategy& Other, T* Ptr)
	{
		Prev = const_cast<CLinkedListStrategy*>(&Other);
		Next = Prev->Next;
		Prev->Next = this;
		Next->Prev = this;		
		return Ptr;
	}

	void InitRef(T* Ptr)
	{
		Next = this;
		Prev = this;
	}

	size_t Release(T* Ptr)
	{
		if (this == Next)
		{
			return 0;
		}
		else
		{
			Prev->Next = Next;
			Next->Prev = Prev;

			Next = this;
			Prev = this;
			return 1;
		}
	}
};

template <class T>
class CCloneBasicStrategy
{
public:

	T* AddRef(const CCloneBasicStrategy& Other, T* Ptr)
	{
		if (!Ptr)
			return 0;

		return new T(*Ptr);
	}

	void InitRef(T* Ptr)
	{
	}

	size_t Release(T* Ptr)
	{
		return 0;
	}
};

template <class T,
	template <class T> class CCounterStrategy>
class CSimpleSmartPtr: public CCounterStrategy<T>
{
	T* Ptr;

	void Attach(const CSimpleSmartPtr& Other)
	{
		Ptr = AddRef(Other, Other.Ptr);
	}

public:

	explicit CSimpleSmartPtr()
	{
		Ptr = 0;
		InitRef(Ptr);
	}

	explicit CSimpleSmartPtr(T* V) : Ptr(V)
	{
		InitRef(Ptr);
	}

	T* operator->() const
	{
		return Ptr;
	}

	CSimpleSmartPtr(const CSimpleSmartPtr& Other)
	{
		Attach(Other);
	}

	CSimpleSmartPtr& operator=(const CSimpleSmartPtr& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Detach();
		Attach(Other);
		return *this;
	}

	CSimpleSmartPtr(const CCounterStrategy<T>& Other, T* V)
	{
		Ptr = AddRef(Other, V);
	}

	~CSimpleSmartPtr()
	{
		Detach();
	}

	void Detach()
	{
		if (Release(Ptr) == 0)
		{
			if (Ptr)
			{
				delete Ptr;
			}
		}
		Ptr = 0;
	}

	T* Get() const
	{
		return Ptr;
	}
};


/**
	Weak = число слабых указателей + 1, если есть хотя бы 1 сильный.
	Count = число сильных указателей.
	Когда объект создается - это сильный указатель, поэтому оба равны 1.
	Если уничтожаются все сильные указатели, то Count = 0, а Weak = число слабых.
*/
template <class T>
class CWeakPtrSupportStrategy
{
	struct CCounter
	{
		size_t Count = 1;
		size_t Weak = 1;
	};
	CCounter* Counter;

public:

	CWeakPtrSupportStrategy()
	{
		Counter = 0;
	}

	T* AddRef(const CWeakPtrSupportStrategy& Other, T* Ptr)
	{
		Counter = Other.Counter;
		if (Counter)
		{
			Counter->Count++;
		}
		return Ptr;
	}

	T* AddRefWeak(const CWeakPtrSupportStrategy& Other, T* Ptr)
	{
		Counter = Other.Counter;
		if (Counter)
		{
			Counter->Weak++;
		}
		return Ptr;
	}
		
	void InitRef(T* Ptr)
	{
		Counter = new CCounter();
		printf("New counter %d, ptr %d\r\n",(int)Counter,(int)Ptr);
	}

	bool HasPtr() const
	{
		if (!Counter)
			return false;
		return Counter->Count > 0;
	}

	/*
	size_t Release(T* Ptr)
	{
		if (!Counter)
		{
			return 0;
		}

		Counter->Count--;

		if (Counter->Count == 0)
		{
			Counter->Weak--;
		};

		if (Counter->Weak == 0)
		{
			printf("Delete counter %d\r\n", (int)Counter);
			delete Counter;
			Counter = 0;
			return 0;
		}

		return 1;
	}

	size_t ReleaseWeak(T* Ptr)
	{
		if (!Counter)
		{
			return 0;
		}

		Counter->Weak--;
			
		if (Counter->Weak == 0)
		{
			printf("Delete counter %d\r\n", (int)Counter);
			delete Counter;
			Counter = 0;
			return 0;
		}

		return 1;
	}
	*/

	size_t Release(T* Ptr)
	{
		if (!Counter)
		{
			return 0;
		}

		Counter->Count--;

		if (Counter->Count == 0)
		{
			Counter->Weak--;
		};

		if (Counter->Weak == 0)
		{
			printf("Delete counter %d\r\n", (int)Counter->Count);
			delete Counter;
			Counter = 0;
			return 0;
		}

		size_t R = Counter->Count;
		Counter = 0;
		return R;
	}

	size_t ReleaseWeak(T* Ptr)
	{
		if (!Counter)
		{
			return 1;
		}

		Counter->Weak--;

		if (Counter->Weak == 0)
		{
			printf("Delete counter %d\r\n", (int)Counter->Count);
			delete Counter;
			Counter = 0;
			return 1;
		}

		Counter = 0;
		return 1;
	}
};


template <class T,
	template <class T> class CCounterStrategy>
class CSimpleWeakPtr : public CCounterStrategy<T>
{
	T* Ptr;

	void Attach(const CSimpleWeakPtr& Other)
	{
		Ptr = AddRefWeak(Other, Other.Ptr);
	}

public:

	CSimpleWeakPtr()
	{
		Ptr = 0;
	}

	explicit CSimpleWeakPtr(CSimpleSmartPtr<T, CCounterStrategy>& Other)
	{
		Ptr = AddRefWeak(Other, Other.Get());
	}

	CSimpleSmartPtr<T, CCounterStrategy> Lock()
	{
		if (HasPtr())
		{
			printf("Return valid ptr %d\r\n",(int)Ptr);
			return CSimpleSmartPtr<T, CCounterStrategy>(*this, Ptr);
		}
		else
		{
			printf("Return null ptr\r\n");
			return CSimpleSmartPtr<T, CCounterStrategy>();
		}
	};

	CSimpleWeakPtr(const CSimpleWeakPtr& Other)
	{
		Attach(Other);
	}

	CSimpleWeakPtr& operator=(const CSimpleWeakPtr& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Detach();
		Attach(Other);
		return *this;
	}

	CSimpleWeakPtr& operator=(const CSimpleSmartPtr<T, CCounterStrategy>& Other)
	{
		Detach();
		Ptr = AddRefWeak(Other, Other.Get());
		return *this;
	}
	
	~CSimpleWeakPtr()
	{
		Detach();
	}

	void Detach()
	{
		if (ReleaseWeak(Ptr) == 0)
		{
			if (Ptr)
			{
				delete Ptr;
			}
		}
		Ptr = 0;
	}

};

template <class T>
class CEmbeddedCounterStrategy
{
public:

	T* AddRef(const CEmbeddedCounterStrategy& Other, T* Ptr)
	{
		if (Ptr)
			Ptr->Count++;
		return Ptr;
	}

	void InitRef(T* Ptr)
	{
		if (Ptr)
			Ptr->Count = 1;
	}

	size_t Release(T* Ptr)
	{
		if (!Ptr)
			return 0;

		Ptr->Count--;
		return Ptr->Count;
	}
};

template <template <class T> class CCounterStrategy>
class CSimpleSmartPtrTester1 : public CSmartPtrTestObjectCollection
{
public:

	typedef CSimpleSmartPtr<CTestObject, CCounterStrategy> CSmartPtr_;

	void Test()
	{
		CSmartPtrTestObjectTester<CSmartPtr_> Tester;
		Tester.Test(this);
	}

	void TestDefaultConstructor()
	{
		CSmartPtrTestObjectTester<CSmartPtr_> Tester;
		Tester.TestDefaultConstructor();
	}

};


class CWeakPtrCacheTester
{

	class CFile
	{
		wstring FileName;
	public:

		CFile(wstring AFileName) : FileName(AFileName)
		{
			printf("Open file %S\r\n", FileName.c_str());

			//Reading the file into memory.
		}

		~CFile()
		{
			printf("Close file %S\r\n", FileName.c_str());
		}
	};

	CSimpleSmartPtr<CFile, CWeakPtrSupportStrategy> GetFile(wstring FileName)
	{
		static map<wstring, CSimpleWeakPtr<CFile, CWeakPtrSupportStrategy> > Cache;
		static mutex m;

		lock_guard<mutex> lock(m);
		printf("Try lock\r\n");
		auto sp = Cache[FileName].Lock();
		printf("Lock %d\r\n",(int)sp.Get());
		if (!sp.Get())
		{
			sp = CSimpleSmartPtr<CFile, CWeakPtrSupportStrategy>(new CFile(FileName));
			printf("New Ptr\r\n");
			Cache[FileName] = sp;
			printf("Cache updated\r\n");
		}

		return sp;
	}

public:

	void Test()
	{
		printf("Weak Ptr Tester\r\n");

		{
			auto File = GetFile(L"1.txt");
			printf("Get file 1\r\n");
			auto File1 = GetFile(L"1.txt");
			printf("Get file 2\r\n");
		}

		printf("New block\r\n");

		auto File2 = GetFile(L"1.txt");
	}
};

class CSimpleSmartPtrTester
{
public:

	void Test()
	{
		printf("\r\n\r\nCounter strategy\r\n");
		CSimpleSmartPtrTester1<CCounterStrategy> T1;
		T1.Test();

		printf("\r\n\r\nLinked list strategy\r\n");
		CSimpleSmartPtrTester1<CLinkedListStrategy> T2;
		T2.Test();

		printf("\r\n\r\nClone basic strategy\r\n");
		CSimpleSmartPtrTester1<CCloneBasicStrategy> T3;
		T3.Test();

		printf("\r\n\r\nEmbedded counter strategy\r\n");
		CSimpleSmartPtrTester1<CEmbeddedCounterStrategy> T4;
		T4.Test();

		printf("\r\n\r\nWeak ptr support strategy\r\n");
		CSimpleSmartPtrTester1<CWeakPtrSupportStrategy> T5;
		T5.Test();

		{
			printf("\r\nWeak ptr test\r\n");
			CSimpleSmartPtr<int, CWeakPtrSupportStrategy> Ptr(new int);
			CSimpleWeakPtr<int, CWeakPtrSupportStrategy> Ptr2(Ptr);

			printf("\r\nReset ptr\r\n");
			Ptr = CSimpleSmartPtr<int, CWeakPtrSupportStrategy>(new int);
						
			printf("\r\nPtr has been reset\r\n");
			CSimpleSmartPtr<int, CWeakPtrSupportStrategy> Ptr3 = Ptr2.Lock();
		}

		CWeakPtrCacheTester WeakPtrCacheTester;
		WeakPtrCacheTester.Test();
	}

	void TestDefaultConstructor()
	{
		printf("\r\n\r\nCounter strategy\r\n");
		CSimpleSmartPtrTester1<CCounterStrategy> T1;
		T1.TestDefaultConstructor();

		printf("\r\n\r\nLinked list strategy\r\n");
		CSimpleSmartPtrTester1<CLinkedListStrategy> T2;
		T2.TestDefaultConstructor();

		printf("\r\n\r\nClone basic strategy\r\n");
		CSimpleSmartPtrTester1<CCloneBasicStrategy> T3;
		T3.TestDefaultConstructor();

		printf("\r\n\r\nEmbedded counter strategy\r\n");
		CSimpleSmartPtrTester1<CEmbeddedCounterStrategy> T4;
		T4.TestDefaultConstructor();
	}
};
