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

/**
	Пример двоичного числа на списке.
*/
template <class T>
class CBinaryNumber
{
	CList<T> List;

	/*
		Идем от начала пока не встретим 0.
		Все 1 заменяем на 0.
		Когда встретили 0 или закончили, ставим 1.

		100001111 -> 100010000
		100001010 -> 10001011
		10 -> 11
		1 -> 10
	*/
	CList<T> Inc(CList<T> List)
	{
		if (List.IsEmpty())
		{
			return List.PushFront(1);
		}

		T Front = List.Front();
		if (Front)
		{
			return Inc(List.Tail()).PushFront(0);
		}
		else
		{
			return List.Tail().PushFront(1);
		}
	}

	/*
		Идем от начала пока не встретим 1.
		Все 0 заменяем на 1.
		Когда встретили 1, заменяем на 0.
		Если список кончился и это последняя единица, стираем ее.

		1000100000 -> 1000011111
		1000 -> 111
		100001 -> 100000
	*/
	CList<T> Dec(CList<T> List)
	{
		/*
			Если число != 0, здесь должна быть единица, которую мы стираем.
		*/
		if (List.Tail().IsEmpty())
		{
			return List.Tail();
		}

		T Front = List.Front();
		if (Front)
		{
			return List.Tail().PushFront(0);
		}
		else
		{
			return Dec(List.Tail()).PushFront(1);
		}
	}


	CBinaryNumber(CList<T> AList): List(AList)
	{

	}
public:

	CBinaryNumber()
	{
	}

	CBinaryNumber Inc()
	{
		return CBinaryNumber(Inc(List));
	}

	CBinaryNumber Dec()
	{
		return CBinaryNumber(Dec(List));
	}

	string ToString()
	{
		return List.Reverse().ToString();
	}


	CBinaryNumber Add(size_t Size) const
	{
		CBinaryNumber V = *this;

		for (size_t i = 0; i < Size; i++)
		{
			V = V.Inc();
		}

		return V;
	}

	CBinaryNumber Sub(size_t Size) const
	{
		CBinaryNumber V = *this;

		for (size_t i = 0; i < Size; i++)
		{
			V = V.Dec();
		}

		return V;
	}

	void Print()
	{
		List.Reverse().ForEach([](int V) {
			printf("%d", V);
			return true;
		});
		printf("\n");
	}
};

class CBinaryNumberTest
{
public:

	void Test()
	{
		CBinaryNumber<int> Number;
		for (int i = 0; i < 10; i++)
		{
			printf("%s\r\n", Number.ToString().c_str());
			Number = Number.Inc();
		}

		for (int i = 0; i < 10; i++)
		{
			Number = Number.Dec();
			printf("%s\r\n", Number.ToString().c_str());
		}

		Number.Add(100).Print();
		Number.Add(100).Sub(2).Print();
	};
};
