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

	
template <class T>
class CSimpleSmartPtr
{
	T* Ptr;
	size_t* Counter;

	void Attach(const CSimpleSmartPtr& Other)
	{
		Counter = Other.Counter;
		(*Counter)++;
		Ptr = Other.Ptr;
	}

public:

	explicit CSimpleSmartPtr(T* V) : Ptr(V)
	{
		Counter = new size_t();
		*Counter = 1;
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

	~CSimpleSmartPtr()
	{
		Detach();
	}

	void Detach()
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

};

template <class T>
class CSimpleSmartPtrLocker
{
	T* Ptr;

	struct CCounter
	{
		size_t Count;
		mutex m;
	};

	CCounter* Counter;

	void Attach(const CSimpleSmartPtrLocker& Other)
	{
		Counter = Other.Counter;
		Counter->Count++;
		Ptr = Other.Ptr;
	}

	class CLocker
	{
		CSimpleSmartPtrLocker* Owner;
	public:

		/*
			Если никакие конструкторы копирования, перемещения и операторы
			не определять, тоже вроде работает.
		*/
		CLocker& operator=(const CLocker& Locker) = delete;		
		CLocker(const CLocker& Locker) = delete;

		CLocker(const CLocker &&Locker)
		{
			printf("Move Locker\r\n");
			Owner = Locker.Owner;
			Locker.Owner = 0;
		}

		CLocker(CSimpleSmartPtrLocker* AOwner) : Owner(AOwner)
		{
			printf("Lock\r\n");
			Owner->Counter->m.lock();
		}

		~CLocker()
		{
			if (Owner)
			{
				Owner->Counter->m.unlock();
				printf("Un Lock\r\n");
			}
		}

		T* operator->()
		{
			printf("Get\r\n");
			return Owner->Ptr;
		}
	};
		
public:

	explicit CSimpleSmartPtrLocker(T* V) : Ptr(V)
	{
		Counter = new CCounter();
		Counter->Count = 1;
	}

	CLocker operator->()
	{
		return CLocker(this);
	}

	CSimpleSmartPtrLocker(const CSimpleSmartPtrLocker& Other)
	{
		Attach(Other);
	}

	CSimpleSmartPtrLocker& operator=(const CSimpleSmartPtrLocker& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Detach();
		Attach(Other);
		return *this;
	}

	~CSimpleSmartPtrLocker()
	{
		Detach();
	}

	void Detach()
	{
		if (Ptr)
		{
			Counter->Count--;
			if (Counter->Count == 0)
			{
				delete Ptr;
				delete Counter;
			}
			Ptr = 0;
			Counter = 0;
		}
	}

};

class CSimpleSmartPtrTester : public CSmartPtrTestObjectCollection
{
public:

	typedef CSimpleSmartPtr<CTestObject> CSmartPtr_;

	void Test()
	{
		printf("\r\n\r\nSimple smart ptr\r\n");
		CSmartPtrTestObjectTester<CSmartPtr_> Tester;
		Tester.Test(this);

		printf("\r\n\r\nSimple smart ptr locker\r\n");
		CSmartPtrTestObjectTester<CSimpleSmartPtrLocker<CTestObject>> Tester1;
		Tester1.Test(this);
	}
};


/*
void ExplicitTest1(CSimpleSmartPtr<int> P)
{

}

void ExplicitTest()
{
	int z;
	ExplicitTest1(&z);

	ExplicitTest1(0);
}

*/

