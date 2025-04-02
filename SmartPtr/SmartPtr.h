/*

Пример для курса ФИЛП, УрФУ, 2017-2025. Веретенников А. Б.

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

/**
Опциональный усложненный пример, можно не смотреть.
*/

template <class T, class CTraits>
class CDeleteFreeStrategy
{
public:

	void Free(T* P)
	{
		delete P;
	}
};

template <class T, class CTraits>
class CCounterStrategy
{
	typedef typename CTraits::stored_type stored_type;

	size_t* Value;

public:

	stored_type AddRef(const CCounterStrategy& Other, stored_type Ptr)
	{
		Value = Other.Value;
		size_t R = ++(*Value);
		
		CTraits::AddRef(R);

		return Ptr;
	}

	void InitRef(stored_type Ptr)
	{
		Value = new size_t();
		*Value = 1;

		CTraits::AddRef(1);
	}

	size_t Release(stored_type Ptr)
	{
		if (!Value)
		{
			CTraits::Error("No value");
			return 0;
		}

		size_t R = --(*Value);		

		CTraits::Release(R);

		if (R == 0)
		{
			delete Value;
			Value = 0;
		}
		return R;
	}
};

template <class T, class CTraits>
class CLinkedListStrategy
{
	typedef typename CTraits::stored_type stored_type;

public:

	CLinkedListStrategy* Next;
	CLinkedListStrategy* Prev;

	stored_type AddRef(const CLinkedListStrategy& Other, stored_type Ptr)
	{
		Prev = const_cast<CLinkedListStrategy*>(&Other);
		Next = Prev->Next;
		
		Prev->Next = this;
		Next->Prev = this;

		CTraits::AddRef(1);

		return Ptr;
	}

	void InitRef(stored_type Ptr)
	{
		Next = this;
		Prev = this;

		CTraits::AddRef(1);
	}

	size_t Release(stored_type Ptr)
	{
		size_t R;
		if (this == Next)
		{
			R = 0;
		}
		else
		{
			Prev->Next = Next;
			Next->Prev = Prev;
			R = 1;
		}
		
		CTraits::Release(R);
		return R;
	}
};

template <class T, class CTraits>
class CCloneBasicStrategy
{
	typedef typename CTraits::stored_type stored_type;

public:

	stored_type AddRef(const CCloneBasicStrategy& Other, stored_type Ptr)
	{
		stored_type Value = new CTraits::source_type(*Ptr);

		CTraits::AddRef(1);

		return Value;
	}

	void InitRef(stored_type Ptr)
	{
	
	}

	size_t Release(stored_type Ptr)
	{
		CTraits::Release(0);

		return 0;
	}
};

template <class T>
class CBaseTraits
{
public:
	
	static bool IsDebugEnabled()
	{
		return false;
	}

	static inline void Debug(const char* Message, ...)
	{
		string Str = "[DEBUG] ";
		Str += Message;

		va_list Args;
		va_start(Args, Message);
		vprintf(Str.c_str(), Args);
		va_end(Args);
	}

	static inline void Error(const char* Message, ...)
	{
		string Str = "[ERROR] ";
		Str += Message;

		va_list Args;
		va_start(Args, Message);
		vprintf(Str.c_str(), Args);
		va_end(Args);
	}

	static inline void AddRef(size_t R)
	{
	}

	static inline void Release(size_t R)
	{

	}
};

template <class T>
class CSmartPtrPointerTraits: public CBaseTraits<T>
{
public:

	typedef T source_type;
	typedef T* stored_type;
	typedef T* pointer_type;
		
};

template <class T>
class CSmartPtrPointerDebugTraits : public CBaseTraits<T>
{
public:

	typedef T source_type;
	typedef T* stored_type;
	typedef T* pointer_type;

	static bool IsDebugEnabled()
	{
		return true;
	}

	static inline void AddRef(size_t R)
	{
		printf("AddRef %I64d\r\n", (uint64_t)R);
	}

	static inline void Release(size_t R)
	{
		printf("Release %I64d\r\n", (uint64_t)R);
	}
};

/**
typename CStoragePolicy<T>::StoredType
*/

template <class T, 
	template <class T, class CTraits> class CRefStrategy,
	template <class T, class CTraits> class CFreeStrategy,
	template <class T> class CTraits = CSmartPtrPointerTraits>
class CSmartPtr:
	private CRefStrategy<T, CTraits<T> >,
	private CFreeStrategy<T, CTraits<T> >
{
	typedef typename CTraits<T>::stored_type stored_type;
	typedef typename CTraits<T>::pointer_type pointer_type;

	stored_type Ptr;

	CSmartPtr();

public:

	explicit CSmartPtr(stored_type V) : Ptr(V)
	{
		InitRef(Ptr);
	}

	/*
		const не работает для linked list.

		Нужно проверять параметры.
		Если не совпадают, может быть создан другой конструктор по умолчанию без
		правильной логики.
	*/
	CSmartPtr(const CSmartPtr& Other)
	{
		Ptr = AddRef(Other, Other.Ptr);
	}

	CSmartPtr& operator=(const CSmartPtr& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Detach();
		Ptr = AddRef(Other, Other.Ptr);
		return *this;
	}

	/*
	CSmartPtr(const CSmartPtr<T, CRefStrategy, CFreeStrategy>&& Other)
	{
		printf("Move assignment\r\n");
	}

	CSmartPtr& operator=(const CSmartPtr<T, CRefStrategy, CFreeStrategy>&& Other)
	{
		printf("Move assignment1\r\n");
		return *this;
	}*/

	~CSmartPtr()
	{
		Detach();
	}

	void Detach()
	{
		if (Ptr)
		{
			if (Release(Ptr) == 0)
			{
				Free(Ptr);
			}
			Ptr = 0;
		}
	}

	pointer_type operator->() const
	{
		return Ptr;
	}
};


template <class T, class CTraits>
class CHandleFreeStrategy
{
public:

	void Free(HANDLE P)
	{
		if (CTraits::IsDebugEnabled())
		{
			CTraits::Debug("Close handle %I64d", (uint64_t)P);
		}

		CloseHandle(P);
	}
};

template <class T>
class CSmartPtrHandleTraits: public CBaseTraits<T>
{
public:

	typedef T source_type;
	typedef T stored_type;

	struct CFileHandle
	{
	private:
		HANDLE H;
	public:

		CFileHandle(HANDLE AH): H(AH)
		{
		}

		bool WriteBuffer(const void* Buffer, size_t Size)
		{
			DWORD Written = 0;
			if (!WriteFile(H, Buffer, (DWORD)Size, &Written, 0))
			{
				Error("Write error %d, handle %d", (int)GetLastError(), (int)H);
				return false;
			}
			if (Written != Size)
			{
				Error("Write size mismatch %d != %d", (int)Size, (int)Written);
				return false;
			}
			return true;
		}

		CFileHandle* operator->()
		{
			return this;
		}
	};

	typedef CFileHandle pointer_type;

};

template <class CTraits>
class CCloneBasicStrategy<HANDLE, CTraits>
{
	typedef typename CTraits::stored_type stored_type;

public:

	stored_type AddRef(const CCloneBasicStrategy& Other, stored_type Ptr)
	{
		stored_type Value;
		
		if (!DuplicateHandle(GetCurrentProcess(),
			Ptr,
			GetCurrentProcess(),
			&Value,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS))
		{
			CTraits::Error("Duplication error %d",(int)GetLastError());
			return 0;
		};

		return Value;
	}

	void InitRef(stored_type Ptr)
	{

	}

	size_t Release(stored_type Ptr)
	{
		return 0;
	}
};

template <template <class T, class CTraits> class CRefStrategy> 
class CSmartPtrStrategyTester: public CSmartPtrTestObjectCollection
{
public:	
	
	typedef CSmartPtr<CTestObject, CRefStrategy, CDeleteFreeStrategy, CSmartPtrPointerDebugTraits> CSmartPtr_;

	void Test()
	{
		CSmartPtrTestObjectTester<CSmartPtr_> Tester;
		Tester.Test(this);

		TestF();
	}

	void Write(CSmartPtr<HANDLE, CRefStrategy, CHandleFreeStrategy, CSmartPtrHandleTraits> HPtr)
	{
		int Test = 0;
		bool R = HPtr->WriteBuffer(&Test, sizeof(Test));
		printf("Write result %d\r\n", (int)R);
	}

	void TestF()
	{
		// FILE_ATTRIBUTE_EA ?
		HANDLE H = CreateFile(_TEXT("test.txt"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (H)
		{
			printf("Handle %d\r\n", (int)H);
			CSmartPtr<HANDLE, CRefStrategy, CHandleFreeStrategy, CSmartPtrHandleTraits> HPtr(H);
			Write(HPtr);
			printf("After write\r\n");
		}
		else
		{
			printf("Cannot open file\r\n");
		}
	}
};

class CSmartPtrDetailedTester
{
public:

	void Test()
	{
		printf("Counter\r\n");

		Test1();

		printf("Linked List\r\n");
		Test2();

		printf("Cloned Basic\r\n");
		Test3();
	}

	void Test1()
	{
		CSmartPtrStrategyTester<CCounterStrategy> Tester;
		Tester.Test();
	}

	void Test2()
	{
		CSmartPtrStrategyTester<CLinkedListStrategy> Tester;
		Tester.Test();
	}

	void Test3()
	{
		CSmartPtrStrategyTester<CCloneBasicStrategy> Tester;
		Tester.Test();
	}
};