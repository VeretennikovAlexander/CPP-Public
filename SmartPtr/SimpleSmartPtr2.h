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

/*
  В этом примере добавлены операторы сравнения умных указателей.
  Добавлен оператор преобразования к обычному указателю.
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

        /*
          Определяем два оператора T* и void*, чтобы нельзя было применить delete к умному указателю.
        */
	operator T*()
	{
		return Ptr;
	}

	operator void*()
	{
		return Ptr;
	}

	T* Get() const
	{
		return Ptr;
	}

	explicit operator bool()
	{
		return Ptr != 0;
	}
};

template <class T1, class T2>
bool operator==(const CSimpleSmartPtr<T1>& Left, const CSimpleSmartPtr<T2>& Right)
{
	return Left.Get() == Right.Get();
}

template <class T1, class T2>
bool operator!=(const CSimpleSmartPtr<T1>& Left, const CSimpleSmartPtr<T2>& Right)
{
	return !(Left.Get() == Right.Get());
}

template <class T1, class T2>
bool operator<(const CSimpleSmartPtr<T1>& Left, const CSimpleSmartPtr<T2>& Right)
{
	return Left.Get() < Right.Get();
}
	
template <class T1, class T2>
bool operator>(const CSimpleSmartPtr<T1>& Left, const CSimpleSmartPtr<T2>& Right)
{
	return Right.Get() < Left.Get();
}

template <class T1, class T2>
bool operator<=(const CSimpleSmartPtr<T1>& Left, const CSimpleSmartPtr<T2>& Right)
{
	return !(Right.Get() < Left.Get());
}

template <class T1, class T2>
bool operator>=(const CSimpleSmartPtr<T1>& Left, const CSimpleSmartPtr<T2>& Right)
{
	return !(Left.Get() < Right.Get());
}
	
/*
	Сравнения с nullptr_t.
*/


void f1()
{
	CSimpleSmartPtr<int> Ptr(new int);
	CSimpleSmartPtr<int> Ptr2(new int);

	if (Ptr == Ptr2)
	{

	}
	if (Ptr < Ptr2)
	{

	}

	if (Ptr)
	{

	}

	/*
	error C2440: 'delete': cannot convert 
	from 'SimpleSmartPtr2::CSimpleSmartPtr<int>' to 'void*'
	note: Ambiguous user - defined - conversion
	*/
	//delete Ptr;
}

class CSimpleSmartPtrTester : public CSmartPtrTestObjectCollection
{
public:

	typedef CSimpleSmartPtr<CTestObject> CSmartPtr_;

	void Test()
	{
		CSmartPtrTestObjectTester<CSmartPtr_> Tester;
		Tester.Test(this);
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

