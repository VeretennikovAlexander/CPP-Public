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
class CBinominalHeap
{
	class CNode;

	typedef shared_ptr<CNode> PNode;

	typedef CList<PNode> CNodeList;

	struct CNode
	{
		size_t Rank;
		T Value;
		CNodeList Children;

		CNode(size_t ARank, T AValue, CNodeList AChildren)
		{
			Rank = ARank;
			Value = AValue;
			Children = AChildren;
		}

		CNode(size_t ARank, T AValue)
		{
			Rank = ARank;
			Value = AValue;
		}
	};

	CNodeList Heap;

	/*
		Применяется для слияния двух деревьев одинакового ранга r в дерево ранга (r+1).
		Путем добавления одного из деревьев в качестве первого элемента Children другого дерева.
		В списке Children деревья упорядочены по убыванию ранга.
	*/
	PNode Link(PNode First, PNode Second) const
	{
		if (First->Value < Second->Value)
		{
			return make_shared<CNode>(First->Rank + 1, First->Value, First->Children.PushFront(Second));
		}
		else
		{
			return make_shared<CNode>(Second->Rank + 1, Second->Value, Second->Children.PushFront(First));
		}
	}

	/**
		Ранг дерева Tree должен быть <= ранга всех деревьев в Children.
		В Children деревья упорядочены по возрастанию ранга.
	*/
	CNodeList InsTree(PNode Tree, CNodeList Children) const
	{
		if (Children.IsEmpty())
		{
			return Children.PushFront(Tree);
		}
		PNode Front = Children.Front();
		if (Tree->Rank < Front->Rank)
		{
			return Children.PushFront(Tree);
		}
		else
		{
			return InsTree(Link(Tree, Front), Children.Tail());
		}
	}

	/**
		Вставляет элемент в кучу.
		На основании элемента создаем дерево ранга 0.
		Вставляем его в кучу Children.
		Children - список деревьев, упорядоченный по возрастанию ранга.
	*/
	CNodeList Insert(T Value, CNodeList Children) const
	{
		return InsTree(make_shared<CNode>(0, Value), Children);
	}

	/**
		Оба аргумента - списки деревьев, упорядоченные по возрастанию ранга.
	*/
	CNodeList Merge(CNodeList First, CNodeList Second) const
	{
		if (First.IsEmpty())
		{
			return Second;
		}
		if (Second.IsEmpty())
		{
			return First;
		}
		PNode FirstFront = First.Front();
		PNode SecondFront = Second.Front();
		if (FirstFront->Rank < SecondFront->Rank)
		{
			return Merge(First.Tail(), Second).PushFront(FirstFront);
		} else
		if (SecondFront->Rank < FirstFront->Rank)
		{
			return Merge(Second.Tail(), First).PushFront(SecondFront);
		}
		else
		{
			return InsTree(Link(FirstFront,SecondFront), Merge(First.Tail(), Second.Tail()));
		}
	}

	CBinominalHeap(CNodeList AHeap): Heap(AHeap)
	{
	}

	PNode GetMinNode() const
	{
		return Heap.FoldL<PNode>([](PNode A, PNode B) {
			if (A->Value < B->Value)
			{
				return A;
			}
			else
			{
				return B;
			}
		});
	}
public:

	CBinominalHeap()
	{

	}

	/*
		Note. Insert does not need Merge. Merge is required only for DeleteMin.
	*/
	CBinominalHeap Insert(T Value) const
	{
		return CBinominalHeap(Insert(Value, Heap));
	}

	T GetMin() const
	{
		return GetMinNode()->Value;
	}

	bool IsEmpty() const
	{
		return Heap.IsEmpty();
	}

	/*
		Для дерева узлы в списке Children упорядочены по убыванию ранга, поэтому перед Merge делаем Reverse.
		А в списке Heap деревья упорядочены по возрастанию ранга.
	*/
	CBinominalHeap DeleteMin() const
	{
		PNode MinNode = GetMinNode();
		CNodeList ExtraList = Heap.Filter([MinNode](PNode Node) {
			return Node != MinNode;
		});
		return CBinominalHeap(Merge(ExtraList, MinNode->Children.Reverse()));
	}
};

class CBinominalHeapTest
{
public:

	void Test()
	{
		printf("Binominal Heap Test\r\n");

		CBinominalHeap<int> Heap;
		Heap = Heap.Insert(7);
		Heap = Heap.Insert(3);
		Heap = Heap.Insert(1);
		for (int i = 100; i < 200; i++)
		{
			Heap = Heap.Insert(i);
		}
		Heap = Heap.Insert(10);
		printf("%d\r\n", Heap.GetMin());
		for (size_t i = 0; i < 10; i++)
		{
			Heap = Heap.DeleteMin();
			printf("%d\r\n", Heap.GetMin());
		}
	}
};
