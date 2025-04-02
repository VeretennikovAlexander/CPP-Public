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
  Пример реализации аналога make_shared.
*/

template <class T>
class CSimpleSmartPtr
{
	T* Ptr;
	size_t* Counter;

	void Attach(const CSimpleSmartPtr& Other)
	{
		Counter = Other.Counter;
		Ptr = Other.Ptr;
		if (Counter)
		{
			(*Counter)++;
		}
	}

public:

	CSimpleSmartPtr()
	{
		Counter = 0;
		Ptr = 0;
	}

	explicit CSimpleSmartPtr(T* V) : Ptr(V)
	{
		Counter = new size_t();
		if (((char*)Counter + sizeof(size_t*)) == (char*)Ptr)
		{
			size_t* Counter1 = new size_t();
			delete Counter;
			Counter = Counter1;
		}
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
		if (!Counter)
			return;

		if (Ptr)
		{
			(*Counter)--;
			if ((*Counter) == 0)
			{
				if (((char*)Counter + sizeof(size_t*)) == (char*)Ptr)
				{
					Ptr->~T();
					free(Counter);	
					
					//printf("Common buffer destroy\n");
				}
				else
				{				
					delete Ptr;
					delete Counter;		

					//printf("Two buffers destroy\n");
				}
			}
			Ptr = 0;
			Counter = 0;
		}
	}

	/*
		make_shared example.

                Создаем один буфер, где в начале размещаем счетчик, затем сам объект.
                Таким образом, объект идет непосредственно за счетчиком.

                В альтернативном конструкторе explicit CSimpleSmartPtr(T* V) обеспечивается,
                что при создании счетчика отдельно от объекта, никогда счетчик в памяти
                не располагается непосредственно перед объектом.
                Таким образом при освобождении данных можно определить, каким способом
                было выделено пространство под счетчик.
	*/
	template <class... CArgs>
	static CSimpleSmartPtr<T> Make(CArgs... Args)
	{
		char* Buffer = (char*)malloc(sizeof(T) + sizeof(size_t*));
		char* Buffer1 = Buffer + sizeof(size_t*);
		CSimpleSmartPtr<T> Result;
		Result.Ptr = new (Buffer1) T(Args...);
		Result.Counter = (size_t*)Buffer;
		*Result.Counter = 1;
		return Result;
	}

};


class CSimpleSmartPtrTester : public CSmartPtrTestObjectCollection
{
public:

	typedef CSimpleSmartPtr<CTestObject> CSmartPtr_;

	void Test()
	{
		printf("\r\n\r\nSimple smart ptr\r\n");
		
		CSmartPtrTestObjectCollection Collection;
		CSimpleSmartPtr<CTestObject> Ptr1 = CSimpleSmartPtr<CTestObject>::Make(1, &Collection);
		CSimpleSmartPtr<CTestObject> Ptr2(new CTestObject(2, &Collection));
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

