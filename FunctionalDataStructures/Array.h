
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

template <class T, size_t BitsPerLevel = 5>
class CArray
{
	struct CNode;

	typedef shared_ptr<CNode> PNode;

	enum {NodeSize = 1 << BitsPerLevel};

	struct CInterNode;

	struct CLeaf;

	struct CNode
	{
		virtual ~CNode()
		{

		}

		virtual size_t Type() = 0;

		CLeaf& AsLeaf()
		{
			return *(CLeaf*)this;
		}

		CInterNode& AsInterNode()
		{
			return *(CInterNode*)this;
		}
	};

	struct CInterNode: public CNode
	{
		PNode Nodes[NodeSize];

		virtual size_t Type() 
		{
			return 1;
		}
	};

	struct CLeaf: public CNode
	{
		T Values[NodeSize];

		virtual size_t Type()
		{
			return 0;
		}

	};

	PNode Root;
	size_t Level = 0;
	size_t Count = 0;
	size_t Total = 0;

	size_t GetTotal() const
	{
		return 1 << (BitsPerLevel * Level);
	}

	CArray(PNode ARoot, size_t ALevel, size_t ACount)
	{
		Root = ARoot;
		Level = ALevel;
		Count = ACount;
		Total = GetTotal();
	}

	PNode Push(PNode Node, size_t Position, size_t Index, T Value) const
	{
		size_t SubIndex;

		/*
			Level = 2.
			Position = 1;
			Level - 1 = 1;
			Сдвиг в цикле на BitsPerLevel вправо.
		*/
		
		SubIndex = (Index >> ((Level - Position) * BitsPerLevel)) & (NodeSize - 1);
		PNode NextNode = Node->AsInterNode().Nodes[SubIndex];
		PNode NewNode;

		/*
			Level = 2.
			Position = 1;
			Level - 1 = 1.
			Нужно создавать Leaf.
		*/
		if (Position == (Level - 1))
		{
			if (NextNode)
			{
				NewNode = PNode(new CLeaf(NextNode->AsLeaf()));
			}
			else
			{
				NewNode = PNode(new CLeaf());
			}

			NewNode->AsLeaf().Values[Index & (NodeSize - 1)] = Value;
		}
		else
		{
			if (!NextNode)
				NextNode = PNode(new CInterNode());
						
			NewNode = Push(NextNode, Position + 1, Index, Value);			
		}

		PNode Result = PNode(new CInterNode(Node->AsInterNode()));
		Result->AsInterNode().Nodes[SubIndex] = NewNode;
		return Result;
	}

public:

	CArray()
	{

	}

	T Get(size_t Index) const
	{
		PNode Node = Root;
		size_t SubIndex;
		for (size_t Position = 1; Position < Level; Position++)
		{
			SubIndex = (Index >> ((Level - Position) * BitsPerLevel)) & (NodeSize - 1);
			Node = Node->AsInterNode().Nodes[SubIndex];
		}
		return Node->AsLeaf().Values[Index & (NodeSize - 1)];
	}

	CArray Set(size_t Index, T Value) const
	{
		return CArray(Push(Root, 1, Index, Value), Level, Count);
	}

	CArray Add(T Value) const
	{
		if (Count == 0)
		{
			PNode Node(new CLeaf());
			((CLeaf*)Node.get())->Values[0] = Value;
			return CArray(Node, Level + 1, Count + 1);
		}

		if (Count >= Total)
		{
			PNode Node(new CInterNode());
			((CInterNode*)Node.get())->Nodes[0] = Root;
			return CArray(Node, Level + 1, Count).Add(Value);
		}

		if (Level == 1)
		{
			PNode Node = PNode(new CLeaf(Root->AsLeaf()));
			Node->AsLeaf().Values[Count] = Value;
			return CArray(Node, Level, Count + 1);
		}

		size_t Index = Count;
		return CArray(Push(Root, 1, Index, Value), Level, Count + 1);
	}

	size_t GetLevel() const
	{
		return Level;
	}

};

class CArrayTest
{
	static int Counter;

	struct CItem
	{
		CItem(CItem& Value)
		{
			Counter++;
		}
		
		CItem(size_t i)
		{
			Counter++;
		}

		CItem()
		{
			Counter++;
		}

		~CItem()
		{
			Counter--;
		}
	};
public:

	void Test1()
	{
		{
			CArray<CItem> Array;
			for (size_t i = 0; i < 1000; i++)
			{
				Array = Array.Add(CItem(i));
			}
		}
		printf("Counter %d\r\n", (int)Counter);
	}

	void Test()
	{
		printf("Array\r\n");

		size_t Display = 200;

		CArray<size_t> Array;
		for (size_t i = 0; i < Display; i++)
		{
			Array = Array.Add(i);
		}

		printf("Level %d\r\n", (int)Array.GetLevel());

		Array = Array.Set(10, 10000);

		for (size_t i = 0; i < Display; i++)
		{
			printf("%d ", (int)Array.Get(i));
		}

		printf("\r\n");

		time_t Start = time((time_t*)0);

		Array = CArray<size_t>();
		size_t Check = 2000;
		for (size_t i = 0; i < Check; i++)
		{
			Array = Array.Add(i);
		}
		printf("Level %d\r\n", (int)Array.GetLevel());
		for (size_t i = 0; i < Check; i++)
		{
			if (i != Array.Get(i))
			{
				printf("Check error %d\r\n", (int)i);
				break;
			}
		}
		printf("\r\n");

		time_t End = time((time_t*)0);

		printf("Time %d\r\n", (int)(End - Start));

		Test1();
	}
};

int CArrayTest::Counter = 0;
